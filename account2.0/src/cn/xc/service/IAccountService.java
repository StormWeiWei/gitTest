package cn.xc.service;

public interface IAccountService {
	
	//转账业务逻辑
	public abstract void transfer(String fromName,String toName,double transferMoney);
	
}
