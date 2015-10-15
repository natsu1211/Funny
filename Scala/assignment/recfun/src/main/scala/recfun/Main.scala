package recfun
import common._

object Main {
  def main(args: Array[String]) {
    println("Pascal's Triangle")
    for (row <- 0 to 10) {
      for (col <- 0 to row)
        print(pascal(col, row) + " ")
      println()
    }
  }

  /**
   * Exercise 1
   */
  def pascal(c: Int, r: Int): Int = if(c == 0 || r == 0 || c == r)  1 else pascal(c,r-1)+pascal(c-1,r-1)

  /**
   * Exercise 2
   */
   def balance(chars: List[Char]): Boolean = {
   def balance2(counter: Int, chars: List[Char]): Boolean = {
     if (chars.isEmpty) counter==0
     else if (chars.head == '(') balance2(counter+1, chars.tail)
     else if (counter!=0 && chars.head == ')') balance2(counter-1, chars.tail)
     else if (counter==0 && chars.head == ')') false
     else balance2(counter, chars.tail)
    }
    balance2(0, chars)
  }    

  /**
   * Exercise 3
   */
  def countChange(money: Int, coins: List[Int]): Int = {
    def sum(money: Int, coins: List[Int], curValue: Int, add: Int): Int = {
      if(curValue > money) 0
      else
      count(money, coins, curValue) + sum(money, coins, curValue + add, add)
    }
    def count(money: Int, coins: List[Int], curValue: Int): Int = {
      if (curValue == money) 1
      else if (coins.isEmpty) 0
      else {
      sum(money, coins.tail, curValue, coins.head)
      }
  }
  count(money, coins, 0)
}                       
}
