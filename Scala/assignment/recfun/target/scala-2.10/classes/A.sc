object A {
  println("Welcome to the Scala worksheet")       //> Welcome to the Scala worksheet
  def pascal(c: Int, r: Int): Int = {
      if(c == 0 || r == 0 || c == r)  1 else pascal(c,r-1)+pascal(c-1,r-1)
  }                                               //> pascal: (c: Int, r: Int)Int
  
  pascal(2,4)                                     //> res0: Int = 6
  
  def balance(chars: List[Char]): Boolean = {
   def balance2(counter: Int, chars: List[Char]): Boolean = {
     if (chars.isEmpty == true ) counter==0
     else if (chars.head == '(') balance2(counter+1, chars.tail)
     else if (counter!=0 && chars.head == ')') balance2(counter-1, chars.tail)
     else if (counter==0 && chars.head == ')') false
     else balance2(counter, chars.tail)
    }
    balance2(0, chars)
  }                                               //> balance: (chars: List[Char])Boolean
 
 balance("(()))".toList)                          //> res1: Boolean = false
 balance("I told him (that it's not (yet) done).\n(But he wasn't listening)".toList)
                                                  //> res2: Boolean = true

  
  
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
}                                                 //> countChange: (money: Int, coins: List[Int])Int

countChange(300,List(500,5,50,100,20,200,10))     //> res3: Int = 1022
}