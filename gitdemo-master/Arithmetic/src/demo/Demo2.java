package demo;

import java.util.Scanner;
	/**
	 *   问题原型：
	 *   传说在很久很久以前，有一架搭载着n个人的飞机出现了故障，迫降在了一个荒岛上。飞机彻底报废后，
	 *   这些人用飞机的残骸建成了一艘只能容纳一个人乘坐的小船，那么怎么去确定这n个人中哪个人有资格上船呢？
	 *   于是，这n个人采用了下面的方式来解决这个困境。
 	 *  问题描述：
	 *   假设有N个人围成一圈，每个人都有从1到N的唯一顺序编号。接下来从编号为1的人开始顺序报数。
	 *   报到M号的人退出这个圈。然后由下一个人重新由1开始进行报数，
	 *   如此循环往复，那么最终留下来的是编号为多少的那个人呢？
	 *   1.程序分析,约瑟夫环问题
	 *   约瑟夫环(约瑟夫问题)是一个数学的应用问题:已知n个人(以编号1，2，3...n分别表示)围坐在一张圆桌周围。从编号为k的人开始报数，
	 *   数到m的那个人出列;他的下一个人又从1开始报数，数到m的那个人又出列;依此规律重复下去，直到圆桌周围的人全部出列。
	 * @param args
	 */
public class Demo2 {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		// 接收用户输入，获得总人数N
		System.out.println("请输入参与的总人数：");
		int N = sc.nextInt();
		// 接收用户输入，获得出列人员的编号M
		System.out.println("请输入出列的人的编号：");
		int M = sc.nextInt();
		// 建立布尔型的数组，长度为总人数
		boolean rs[] = new boolean[N];
		// 初始化布尔型数组，初始值均为true
		for (int i = 0; i < N; i++) {
			rs[i] = true;		
		}
		int n = N;// 剩余的人数
		int m = 0;// 报数的编号
		while (n > 1) {
			for (int j = 0; j < N; j++) {
				//如果数组元素为true
				if (rs[j]) {
					m++;
					if (m == M) {
						m = 0;
						rs[j] = false;
						n--;						
					}
				}
			}
		}
		// 打印出最后留下来的人员的编号
		for (int k = 0; k < N; k++) {
			if (rs[k]) {
				System.out.println("最后留下的是第" + (k + 1) + "号。");
				break;
			}
		}
	}
}