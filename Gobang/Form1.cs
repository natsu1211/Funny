using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GoBang
{
    interface IBoardView
    {
        void UpdateView(Object sender, ModelChangedEventArgs e);
    }
    public partial class Form1 : Form, IBoardView
    {
        private Graphics g;
        private Graphics bufferGraphics;
        private Bitmap bmp;
        //not a good idea
        private GridObject[] b;
        public const int startX = 30;
        public const int startY = 30;
        IBoardGame<GridObject> gameModel;

        public Form1()
        {
            InitializeComponent();
            b =new GridObject[]{};
            int gridlength = (ClientRectangle.Height -  startY) / 15;
            gameModel = new GoBangModel(gridlength);
            gameModel.ModelChanged += this.UpdateView;
            g = CreateGraphics();
            
        }

        public void UpdateView(Object sender, ModelChangedEventArgs e)
        {
            b = e.board;
            DrawChess();
        }
        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            // this function will invoke event Modelchanged
            int x = (int)((float)e.X / (float)gameModel.GridLength - 0.5);
            int y = (int)((float)e.Y / (float)gameModel.GridLength - 0.5);
            if(e.Button == MouseButtons.Left && GridObject.Empty == gameModel.GetGridStatus(x,y))
            {
                gameModel.AddObject(x, y);
                var isGameOver = gameModel.IsGameOver(x, y);
                switch (isGameOver)
                {
                    case GameStatus.Continue:
                        {
                            break;
                        }
                    case GameStatus.BlackWin:
                        {
                            if (MessageBox.Show("Black Win!") == DialogResult.OK)
                                gameModel.ResetGame();
                            break;
                        }
                    case GameStatus.WhiteWin:
                        {
                            if (MessageBox.Show("White Win!") == DialogResult.OK)
                                gameModel.ResetGame();
                            break;
                        }
                    case GameStatus.Draw:
                        {
                            if (MessageBox.Show("Draw!") == DialogResult.OK)
                                gameModel.ResetGame();
                            break;
                        }
                    default: break;
                }
                if (isGameOver != GameStatus.Continue)
                    return;
                var move = gameModel.AIMove();
                switch (gameModel.IsGameOver(move.Item1, move.Item2))
                {
                    case GameStatus.Continue:
                        {
                            break;
                        }
                    case GameStatus.BlackWin:
                        {
                            if (MessageBox.Show("Black Win!") == DialogResult.OK)
                                gameModel.ResetGame();
                            break;
                        }
                    case GameStatus.WhiteWin:
                        {
                            if (MessageBox.Show("White Win!") == DialogResult.OK)
                                gameModel.ResetGame();
                            break;
                        }
                    case GameStatus.Draw:
                        {
                            if (MessageBox.Show("Draw!") == DialogResult.OK)
                                gameModel.ResetGame();
                            break;
                        }
                    default: break;
                }
            }
                
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            DrawBoard();
            DrawChess();
        }

        private void button1_MouseClick(object sender, MouseEventArgs e)
        {
            gameModel.ResetGame();
        }

        private void button2_MouseClick(object sender, MouseEventArgs e)
        {
            gameModel.Retract();
        }

        private void DrawBoard()
        {
            //draw background
            bmp = new Bitmap(ClientRectangle.Width, ClientRectangle.Height);
            bufferGraphics = Graphics.FromImage(bmp);
            Color color1 = Color.FromArgb(255, 153, 18);
            Color color2 = Color.FromArgb(205, 103, 18);
            var bgBrush = new LinearGradientBrush(new Point(0, 0), new Point(ClientRectangle.Right - ClientRectangle.Left, 0), color1, color2);
            bufferGraphics.FillRectangle(bgBrush, ClientRectangle);
            Pen blackPenW2 = new Pen(Color.Black, 2);
            Pen blackPenW4 = new Pen(Color.Black, 4);
            //draw vertical lines of chessboard
            for (int i = 0; i < gameModel.DimensionX; ++i)
            {
                if (0 == i || gameModel.DimensionX - 1 == i)
                    bufferGraphics.DrawLine(blackPenW4, startX + i * gameModel.GridLength, startY,
                    startX + i * gameModel.GridLength, startY + (gameModel.DimensionY - 1) * gameModel.GridLength);
                else bufferGraphics.DrawLine(blackPenW2, startX + i * gameModel.GridLength, startY,
                    startX + i * gameModel.GridLength, startY + (gameModel.DimensionY - 1) * gameModel.GridLength);
            }
            //draw horizontal lines of chessboard
            for (int j = 0; j < gameModel.DimensionY; ++j)
            {
                if (0 == j || gameModel.DimensionX - 1 == j)
                    bufferGraphics.DrawLine(blackPenW4, startX, startY + j * gameModel.GridLength,
                    startX + (gameModel.DimensionX - 1) * gameModel.GridLength, startY + j * gameModel.GridLength);
                else bufferGraphics.DrawLine(blackPenW2, startX, startY + j * gameModel.GridLength,
                    startX + (gameModel.DimensionX - 1) * gameModel.GridLength, startY + j * gameModel.GridLength);
            }
            //draw 5 tengen
            SolidBrush blackBrush = new SolidBrush(Color.Black);
            int[] tengenPos = { 3, 3, 11, 11, 3, 11, 11, 3, 7, 7 };
            for (int i = 0; i < tengenPos.Length; )
            {
                bufferGraphics.FillEllipse(blackBrush,
                    startX + tengenPos[i++] * gameModel.GridLength - gameModel.GridLength / 6,
                    startY + tengenPos[i++] * gameModel.GridLength - gameModel.GridLength / 6,
                    gameModel.GridLength / 3,
                    gameModel.GridLength / 3);
            }
            // g.DrawImage(bmp, ClientRectangle);  
            // bmp.Dispose(); 

        }
        private void DrawChess()
        {
            SolidBrush blackBrush = new SolidBrush(Color.Black);
            SolidBrush whiteBrush = new SolidBrush(Color.White);
            for (int i = 0; i < b.Length; ++i)
            {
                int x = i % gameModel.DimensionX;
                int y = i / gameModel.DimensionX;
                if (GridObject.BlackStone == b[i])
                {

                    bufferGraphics.FillEllipse(blackBrush,
                        startX + x * gameModel.GridLength - gameModel.GridLength / 4,
                        startY + y * gameModel.GridLength - gameModel.GridLength / 4,
                        gameModel.GridLength / 2,
                        gameModel.GridLength / 2);
                }
                if (GridObject.WhiteStone == b[i])
                {

                    bufferGraphics.FillEllipse(whiteBrush,
                        startX + x * gameModel.GridLength - gameModel.GridLength / 4,
                        startY + y * gameModel.GridLength - gameModel.GridLength / 4,
                        gameModel.GridLength / 2,
                        gameModel.GridLength / 2);
                }
            }
            g.DrawImage(bmp, ClientRectangle);
        }

        private void button3_MouseClick(object sender, MouseEventArgs e)
        {
            var settingForm = new Form2();
            settingForm.ShowDialog(this);
        }


    }
}
