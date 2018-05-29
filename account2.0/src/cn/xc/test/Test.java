package cn.xc.test;

import cn.xc.service.IAccountService;
import cn.xc.service.IAccountServiceImpl;

public class Test {

	public static void main(String[] args) {
		
		IAccountService service = new IAccountServiceImpl();
		service.transfer("李四", "张三", 100);
	}
	
}
