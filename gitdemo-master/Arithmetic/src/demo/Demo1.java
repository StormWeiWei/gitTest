package demo;

import java.util.Scanner;
	/**
 	*题目：输入两个正整数m和n，求其最大公约数和最小公倍数。
 	*1.程序分析：利用辗除法。
 	*最大公约数，也称最大公因数、最大公因子，指两个或多个整数共有约数中最大的一个。
 	*最小公倍数=两数的乘积/最大公约（因）数
 	*/
public class Demo1 {
	//最大公约数计算方法
	 public static int commonDivisor(int x,int y){
	    //123456
		 //123
		 //如果x小于y将两个值进行交换
	        if(x < y){
	            int t = x;
	            x = y;
	            y = t;
	        }
	        while(y != 0){
	            if(x == y) {	            		            
	            	return x;
	            }
	            else{
	            	//辗除法,举个简单的例子,24和8的最大公约数
	                int k = x % y;
	                x = y;
	                y = k;
	            }
	        }
	        return x;
	    }
    public static void main(String[] args){
    	//定义最大公约数和最小公倍数
        int m,n;
        Scanner in = new Scanner(System.in);
        System.out.println("请输入一个正整数:");
        int a = in.nextInt();
        System.out.println("再输入一个正整数:");
        int b = in.nextInt();     
        //最大公约数计算
        m = commonDivisor(a,b);
        //最小公倍数计算
        n = a*b / m;
        System.out.println("最大公约数:"+m);
        System.out.println("最小公倍数:"+n);        
    }
    
}

