object A {;import org.scalaide.worksheet.runtime.library.WorksheetSupport._; def main(args: Array[String])=$execute{;$skip(54); 
  println("Welcome to the Scala worksheet");$skip(117); 
  def pascal(c: Int, r: Int): Int = {
      if(c == 0 || r == 0 || c == r)  1 else pascal(c,r-1)+pascal(c-1,r-1)
  };System.out.println("""pascal: (c: Int, r: Int)Int""");$skip(17); val res$0 = 
  
  pascal(2,4);System.out.println("""res0: Int = """ + $show(res$0));$skip(425); 
  
  def balance(chars: List[Char]): Boolean = {
   def balance2(counter: Int, chars: List[Char]): Boolean = {
     if (chars.isEmpty == true ) counter==0
     else if (chars.head == '(') balance2(counter+1, chars.tail)
     else if (counter!=0 && chars.head == ')') balance2(counter-1, chars.tail)
     else if (counter==0 && chars.head == ')') false
     else balance2(counter, chars.tail)
    }
    balance2(0, chars)
  };System.out.println("""balance: (chars: List[Char])Boolean""");$skip(27); val res$1 = 
 
 balance("(()))".toList);System.out.println("""res1: Boolean = """ + $show(res$1));$skip(85); val res$2 = 
 balance("I told him (that it's not (yet) done).\n(But he wasn't listening)".toList);System.out.println("""res2: Boolean = """ + $show(res$2));$skip(498); 

  
  
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
};System.out.println("""countChange: (money: Int, coins: List[Int])Int""");$skip(47); val res$3 = 

countChange(300,List(500,5,50,100,20,200,10));System.out.println("""res3: Int = """ + $show(res$3))}
}
