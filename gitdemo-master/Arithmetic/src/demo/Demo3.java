package demo;

import java.util.Scanner;
	/**
	 * 题目：输入某年某月某日，判断这一天是这一年的第几天？
	 * 1.程序分析：以3月5日为例，应该先把前两个月的加起来， 然后再加上5天即本年的第几天，
	 * 特殊情况，闰年且输入月份大于3时需考虑多加一天。	
	 */
public class Demo3 {
	
	public static void main(String[] args) {
		//定义变量
		int year; int month;int day;
		//输入设备
		Scanner in = new Scanner(System.in);
		System.out.println("请输入年份(必须是正整数):");
		 year = in.nextInt();
		System.out.println("请输入月份(正确的月份):");
		 month = in.nextInt();
		 System.out.println("请输入那一天(正确的天数):");
		 day = in.nextInt();
		 if((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
			 System.out.println("您输入的年份是闰年。");
		 switch(month) {
		 case 1:day+=0;break;
		 case 2:day+=31;break;
		 case 3:day+=60;break;
		 case 4:day+=91;break;
		 case 5:day+=121;break;
		 case 6:day+=152;break;
		 case 7:day+=182;break;
		 case 8:day+=213;break;
		 case 9:day+=244;break;
		 case 10:day+=274;break;
		 case 11:day+=305;break;
		 case 12:day+=335;break;
		
		 default:System.out.println("您输入的月份有误，请重新输入！");
		 	}
		 System.out.println("这天是这一年中的第"+day+"天");
		 }
		 else{
			 
			 switch(month) {
			 case 1:day+=0;break;
			 case 2:day+=31;break;
			 case 3:day+=59;break;
			 case 4:day+=90;break;
			 case 5:day+=120;break;
			 case 6:day+=151;break;
			 case 7:day+=181;break;
			 case 8:day+=212;break;
			 case 9:day+=243;break;
			 case 10:day+=373;break;
			 case 11:day+=304;break;
			 case 12:day+=334;break;
			 default:System.out.println("您输入的月份有误，请重新输入！");
			 	}
			 System.out.println("这天是这一年中的第"+day+"天");
		 }
	}
}
