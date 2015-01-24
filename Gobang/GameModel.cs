using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GoBang
{
    interface IChessBoard
    {
       int DimensionX { get; }
       int DimensionY { get; }
       int GridLength { get; }
    }
    interface IBoardGame<T> : IChessBoard
    {
        void AddObject(int x, int y);
        Tuple<int, int> AIMove();
        T GetGridStatus(int x, int y);
        void Retract();
        void ResetGame();
        
        GameStatus IsGameOver(int x, int y);
        event ModelChangedEventHandler ModelChanged;

    }
    public class ModelChangedEventArgs : EventArgs
    {
        public readonly GridObject[] board;
        public ModelChangedEventArgs(GridObject[] board)
        {
            this.board = board;
        }
    }
    public delegate void ModelChangedEventHandler(Object sender, ModelChangedEventArgs e);
    public enum GameStatus
    {
        BlackWin,
        WhiteWin,
        Draw,
        Continue
    };
    public class ChessBoard<T>:IChessBoard
    {
        internal T[] board;
        private int dimensionX, dimensionY, gridLength;
        //public T[] Board{get;set;}
        public int DimensionX { get { return dimensionX; } }
        public int DimensionY { get { return dimensionY; } }
        public int GridLength { get { return gridLength; } }

        public ChessBoard(int dx, int dy, int gridlength, T repeatedValue)
        {
            dimensionX = dx;
            dimensionY = dy;
            gridLength = gridlength;
            //flat array
            board = new T[DimensionX*DimensionY];
            for(int n=0;n<board.Length;++n)
            {
                board[n] = repeatedValue;
            }
        }
 
    }
    public enum GridObject
    {
        Empty,
        BlackStone,
        WhiteStone
    }
    public enum GameMode
    {
        PvP,
        PvC
    }
    public class GoBangModel : IBoardGame<GridObject>
    {
        private ChessBoard<GridObject> gobangBoard;
        private Stack<Tuple<int, int>> stoneStack;
        private int stoneCount;
        private GameMode gameMode;
        public GoBangModel(int gridlength)
        {
            //the formal borad of gobang has 15x15 grids
            gobangBoard = new ChessBoard<GridObject>(15, 15, gridlength, GridObject.Empty);
            //the stack for retract, which will record the sequence of every move
            stoneStack = new Stack<Tuple<int, int>>();
            stoneCount = 0;
            gameMode = GameMode.PvC;
        }
        public int DimensionX { get { return gobangBoard.DimensionX; } }
        public int DimensionY { get { return gobangBoard.DimensionY; } }
        public int GridLength { get { return gobangBoard.GridLength; } }


        public event ModelChangedEventHandler ModelChanged;
        //  public event EventHandler ModelChanged;


        protected virtual void OnModelChanged(ModelChangedEventArgs e)
        {
            if (ModelChanged != null)
            {
                ModelChanged(this, e);
            }
        }

        public void AddObject(int x, int y)
        {
            if (0 <= x && x < DimensionX && 0 <= y && y < DimensionY)
            {
                ++stoneCount;
                //black stone
                if (1 == stoneCount % 2)
                    SetGridStatus(x,y,GridObject.BlackStone);
                else 
                    SetGridStatus(x,y,GridObject.WhiteStone);

                stoneStack.Push(Tuple.Create(x, y));
                OnModelChanged(new ModelChangedEventArgs(gobangBoard.board));
                
            }


        }
        private void SetGridStatus(int x, int y, GridObject obj)
        {
            gobangBoard.board[y * gobangBoard.DimensionX + x] = obj;
        }
        public Tuple<int, int> AIMove()
        {
            if (gameMode == GameMode.PvC)
            {
                ++stoneCount;
                Tuple<int, int> bestPoint = new Tuple<int, int>(-1, -1);
                AlphaBetaMax(-10000000, 10000000, 2, ref bestPoint);
                gobangBoard.board[bestPoint.Item2 * DimensionX + bestPoint.Item1] = GridObject.WhiteStone;
                stoneStack.Push(Tuple.Create(bestPoint.Item1, bestPoint.Item2));
                OnModelChanged(new ModelChangedEventArgs(gobangBoard.board));
                return bestPoint;
            }
            return new Tuple<int, int>(-1, -1);
        }
        public void ResetGame()
        {
            stoneCount = 0;
            for (int n = 0; n < gobangBoard.board.Length; ++n)
            {
                gobangBoard.board[n] = GridObject.Empty;
            }
            stoneStack.Clear();
            OnModelChanged(new ModelChangedEventArgs(gobangBoard.board));
        }
        public void Retract()
        {
            if (stoneCount >= 0)
            {
                --stoneCount;
                var s = stoneStack.Pop();
                gobangBoard.board[s.Item2 * DimensionX + s.Item1] = GridObject.Empty;
                OnModelChanged(new ModelChangedEventArgs(gobangBoard.board));
            }
        }
        public GridObject GetGridStatus(int x, int y)
        {
            return gobangBoard.board[y * gobangBoard.DimensionX + x];
        }

        public GameStatus IsGameOver(int x, int y)
        {
            if (stoneCount == DimensionX * DimensionY)
                return GameStatus.Draw;
            int notUsed = 0;
            if (CrossCount1(x, y, ref notUsed, ref notUsed) == 5 || CrossCount2(x, y, ref notUsed, ref notUsed) == 5 || HorizontalCount(x, y, ref notUsed) == 5 || VerticalCount(x, y, ref notUsed) == 5)
            {
                if (stoneCount % 2 == 1)
                    return GameStatus.BlackWin;
                else return GameStatus.WhiteWin;
            }

            return GameStatus.Continue;
        }
        //maybe not a good design
        private int CrossCount1(int x, int y, ref int mostLeftX, ref int mostLeftY)
        {
            int count = 1;
            int xs = x, ys = y;
            var status = GetGridStatus(x,y);
            if (status == GridObject.Empty)
                return 0;
            while ((--y >= 0) && (++x < DimensionX) && GetGridStatus(x, y) == status)
            {
                ++count;
            }
            //these two paremeters is needed by EvaluateGrid()

            while ((++ys < DimensionY) && (--xs >= 0) && GetGridStatus(xs, ys) == status)
            {
                ++count;
            }
            mostLeftX = ++xs;
            mostLeftY = --ys;
            return count;
        }
        private int CrossCount2(int x, int y, ref int mostLeftX, ref int mostLeftY)
        {
            int count = 1;
            int xs = x, ys = y;
            var status = gobangBoard.board[y * DimensionX + x];
            if (status == GridObject.Empty)
                return 0;
            while ((--y >= 0) && (--x >= 0) && gobangBoard.board[y * DimensionX + x] == status)
            {
                ++count;
            }
            mostLeftX = ++x;
            mostLeftY = ++y;
            while ((++ys < DimensionY) && (++xs < DimensionX) && gobangBoard.board[ys * DimensionX + xs] == status)
            {
                ++count;
            }
            return count;
        }
        private int HorizontalCount(int x, int y, ref int mostLeftX)
        {
            int count = 1;
            int xs = x, ys = y;
            var status = gobangBoard.board[y * DimensionX + x];
            if(status == GridObject.Empty )
                return 0;
            while ((--x >= 0) && gobangBoard.board[y * DimensionX + x] == status)
            {
                ++count;
            }
            mostLeftX = ++x;
            while ((++xs < DimensionX) && gobangBoard.board[ys * DimensionX + xs] == status)
            {
                ++count;
            }
            return count;
        }
        private int VerticalCount(int x, int y, ref int mostUpY)
        {
            int count = 1;
            int xs = x, ys = y;
            var status = gobangBoard.board[y * DimensionX + x];
            if (status == GridObject.Empty)
                return 0;
            while ((--y >= 0) && gobangBoard.board[y * DimensionX + x] == status)
            {
                ++count;
            }
            mostUpY = ++y;
            while ((++ys < DimensionY) && gobangBoard.board[ys * DimensionX + xs] == status)
            {
                ++count;
            }
            return count;
        }

        private int[,] gobangScore = { { 2, 10, 80, 500, 10000 }, { 1, 5, 25, 100, 10000 }, { 0, 0, 0, 0, 10000 } };
        private int[,] whiteScore = { { 5, 100, 2500, 50000, 5000000 }, { 1, 25, 500, 5000, 5000000 }, { 0, 0, 0, 0, 5000000 } };
        private int[,] blackScore = { { -25, -500, -10000, -250000,-1000000 }, { -5, -100, -500, -10000, -1000000 }, { 0, 0, 0, 0, -1000000 } };

        
        private int AlphaBetaMax(int alpha, int beta, int depth, ref Tuple<int, int> bestMovePoint)
        {
            if (depth <= 0)
            {
                return Evaluate();
            }
            for (int j = 0; j < DimensionY; ++j)
            {
                for (int i = 0; i < DimensionX; ++i)
                {
                    if (GridObject.Empty == gobangBoard.board[j * DimensionX + i])
                    {
                        gobangBoard.board[j * DimensionX + i] = GridObject.WhiteStone;
                        var Score = AlphaBetaMin(alpha, beta, depth - 1, ref bestMovePoint);
                        gobangBoard.board[j * DimensionX + i] = GridObject.Empty;
                        //beta cut-off, beta is the current best result for "min" side
                        if (Score >= beta)
                        {
                            bestMovePoint = new Tuple<int, int>(i, j);
                            return beta;
                        }
                        //update alpha
                        if (Score > alpha)
                        {
                            bestMovePoint = new Tuple<int, int>(i, j);
                            alpha = Score;
                        }
                        
                        
                    }

                }
            }
            return alpha;

        }

        private int AlphaBetaMin(int alpha, int beta, int depth, ref Tuple<int, int> bestMovePoint)
        {
            if (depth <= 0)
            {
                return Evaluate();
            }
            for (int j = 0; j < DimensionY; ++j)
            {
                for (int i = 0; i < DimensionX; ++i)
                {
                    if (GridObject.Empty == gobangBoard.board[j * DimensionX + i])
                    {
                        gobangBoard.board[j * DimensionX + i] = GridObject.BlackStone;
                        var Score = AlphaBetaMax(alpha, beta, depth - 1, ref bestMovePoint);
                        gobangBoard.board[j * DimensionX + i] = GridObject.Empty;
                        
                        if (Score <= alpha)
                        {
                           bestMovePoint = new Tuple<int, int>(i, j);
                            return alpha;
                        }
                        if (Score < beta)
                        {
                            bestMovePoint = new Tuple<int, int>(i, j);
                            beta = Score;
                        }
                        
                        
                    }

                }
            }
            return beta;
        }

        private int Evaluate()
        {
            int sumScore = 0;
            for (int j = 0; j < DimensionY; ++j)
            {
                for (int i = 0; i < DimensionX; ++i)
                {
                    if (GridObject.Empty != gobangBoard.board[j * DimensionX + i])
                        sumScore += EvaluateGrid(i, j);
                }
            }
            return sumScore;
        }

        private int EvaluateGrid(int x, int y)
        {
            var curStatus = gobangBoard.board[y * DimensionX + x];
            int Score = 0;
            int mostLeftX = 0, mostLeftY = 0, mostUpY = 0, notUsed = 0, sameColorCount = 0, d=0, sameColorCountWithBlank = 0, blockCount = 0;
            bool hasBlank = false;
            d = sameColorCount = VerticalCount(x, y, ref mostUpY);

            //verical
            //only consider the situation that gap one blank position, so we minus 2
            if (mostUpY - 2 < 0)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = VerticalCount(x, mostUpY - 2, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            //check one side
            if (mostUpY - 1 >= 0 && gobangBoard.board[(mostUpY - 1) * DimensionX + x] != curStatus && gobangBoard.board[(mostUpY - 1) * DimensionX + x] != GridObject.Empty)
            {
                ++blockCount;
            }
            //not blocked
            else sameColorCount += sameColorCountWithBlank;
            
            //another side
            if (mostUpY + d + 1 >= DimensionY)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = VerticalCount(x, mostUpY + d + 1, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            if (mostUpY + d < DimensionY && gobangBoard.board[(mostUpY + d) * DimensionX + x] != curStatus && gobangBoard.board[(mostUpY + d) * DimensionX + x] != GridObject.Empty)
            {
                ++blockCount;
            }
            else sameColorCount += sameColorCountWithBlank;

            if (sameColorCount < 5)
            {
                if(blockCount == 0 && hasBlank==true)
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[1, sameColorCount - 1];
                    else
                        Score += whiteScore[1, sameColorCount - 1];
                }
                else
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[blockCount, sameColorCount - 1];
                    else
                        Score += whiteScore[blockCount, sameColorCount - 1];
                }

                
            }
            //else Score += 0, since when the number of continuous stones are bigger than 5, there will be no threat
            //reset local variables
            blockCount = 0;
            hasBlank = false;

            //horizontal
            d = sameColorCount = HorizontalCount(x, y, ref mostLeftX);
            if (mostLeftX - 2 < 0)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = HorizontalCount(mostLeftX - 2, y, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            //check one side
            if (mostLeftX - 1 >= 0 && gobangBoard.board[y * DimensionX + mostLeftX - 1] != curStatus && gobangBoard.board[y * DimensionX + mostLeftX - 1] != GridObject.Empty)
            {
                ++blockCount;
            }
            else sameColorCount += sameColorCountWithBlank;
            
            //another side
            if (mostLeftX + d + 1 >= DimensionX)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = HorizontalCount(mostLeftX + d + 1, y, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            if (mostLeftX + d< DimensionX && gobangBoard.board[y * DimensionX + mostLeftX + d] != curStatus && gobangBoard.board[y * DimensionX + mostLeftX + d] != GridObject.Empty)
            {
                ++blockCount;
            }
            else sameColorCount += sameColorCountWithBlank;

            if (sameColorCount <= 5)
            {
                if (blockCount == 0 && hasBlank == true)
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[1, sameColorCount - 1];
                    else
                        Score += whiteScore[1, sameColorCount - 1];
                }
                else
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[blockCount, sameColorCount - 1];
                    else
                        Score += whiteScore[blockCount, sameColorCount - 1];
                }
            }
            //else Score += 0, since when the number of continuous stones are bigger than 5, there will be no threat
            //reset local variables
            blockCount = 0;
            hasBlank = false;

            //Cross1
            d = sameColorCount = CrossCount1(x, y, ref mostLeftX, ref mostLeftY);
            if (mostLeftX - 2 < 0 || mostLeftY + 2 >= DimensionY)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = CrossCount1(mostLeftX - 2, mostLeftY + 2, ref notUsed, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            //check one side
            if (mostLeftX - 1 >= 0 && mostLeftY + 1 < DimensionY && gobangBoard.board[(mostLeftY + 1) * DimensionX + mostLeftX - 1] != curStatus && gobangBoard.board[(mostLeftY + 1) * DimensionX + mostLeftX - 1] != GridObject.Empty)
            {
                ++blockCount;
            }
            else sameColorCount += sameColorCountWithBlank;

            if (mostLeftX + d + 1 >= DimensionX || mostLeftY - d - 1 < 0)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = CrossCount1(mostLeftX + d + 1, mostLeftY - d - 1, ref notUsed, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            //another side
            if (mostLeftX + d < DimensionX && mostLeftY - d >= 0 && gobangBoard.board[(mostLeftY - d) * DimensionX + mostLeftX + d] != curStatus && gobangBoard.board[(mostLeftY - d) * DimensionX + mostLeftX + d] != GridObject.Empty)
            {
                ++blockCount;
            }
            else
            {
                sameColorCount += sameColorCountWithBlank;
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            if (sameColorCount <= 5)
            {
                if (blockCount == 0 && hasBlank == true)
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[1, sameColorCount - 1];
                    else
                        Score += whiteScore[1, sameColorCount - 1];
                }
                else
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[blockCount, sameColorCount - 1];
                    else
                        Score += whiteScore[blockCount, sameColorCount - 1];
                }
            }
            //else Score += 0, since when the number of continuous stones are bigger than 5, there will be no threat
            //reset local variables
            blockCount = 0;
            hasBlank = false;
            //Cross2
            d = sameColorCount = CrossCount2(x, y, ref mostLeftX, ref mostLeftY);
            if (mostLeftX - 2 < 0 || mostLeftY - 2 < 0)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = CrossCount2(mostLeftX - 2, mostLeftY - 2, ref notUsed, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            }
            //seen as have one side block
            if (mostLeftX - 1 >= 0 && mostLeftY - 1 >= 0 && gobangBoard.board[(mostLeftY - 1) * DimensionX + mostLeftX - 1] != curStatus && gobangBoard.board[(mostLeftY - 1) * DimensionX + mostLeftX - 1] != GridObject.Empty)
            {
                ++blockCount;
            }
            else sameColorCount += sameColorCountWithBlank;

            if (mostLeftX + d + 1 >= DimensionX || mostLeftY + d + 1 >= DimensionY)
                sameColorCountWithBlank = 0;
            else
            {
                sameColorCountWithBlank = CrossCount2(mostLeftX + d + 1, mostLeftY + d + 1, ref notUsed, ref notUsed);
                if (sameColorCountWithBlank != 0)
                    hasBlank = true;
            } 

            if (mostLeftX + d < DimensionX && mostLeftY + d< DimensionY && gobangBoard.board[(mostLeftY + d) * DimensionX + mostLeftX + d] != curStatus && gobangBoard.board[(mostLeftY + d) * DimensionX + mostLeftX + d] != GridObject.Empty)
            {
                ++blockCount;
            }
            else sameColorCount += sameColorCountWithBlank;

            if (sameColorCount <= 5)
            {
                if (blockCount == 0 && hasBlank == true)
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[1, sameColorCount - 1];
                    else
                        Score += whiteScore[1, sameColorCount - 1];
                }
                else
                {
                    if (curStatus == GridObject.BlackStone)
                        Score += blackScore[blockCount, sameColorCount - 1];
                    else
                        Score += whiteScore[blockCount, sameColorCount - 1];
                }
            }
            //else Score += 0, since when the number of continuous stones are bigger than 5, there will be no threat
            //reset local variables
            blockCount = 0;
            hasBlank = false;
            return Score;
            

        }
    }

}
