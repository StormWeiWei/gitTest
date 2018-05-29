package cn.xc.service;

import java.sql.SQLException;

import cn.xc.dao.IAccountDao;
import cn.xc.dao.IAccountDaoImpl;
import cn.xc.entity.Account;
import cn.xc.util.DBUtils;

public class IAccountServiceImpl implements IAccountService{

	//转账业务操作
	
	@Override
	public void transfer(String fromName, String toName, double transferMoney) {
		// TODO Auto-generated method stub
		//根据姓名查询账户信息
		try{
			//开启事务
			DBUtils.beginTransaction();
			//多态
			IAccountDao accountDao = new IAccountDaoImpl();
			//付款方
			Account fromAccount = accountDao.queryAccountByName(fromName);
			//收款方
			Account toAccount = accountDao.queryAccountByName(toName);
			//转账
			if(transferMoney < fromAccount.getBalance()){
				//付款方减少
				double fromBalance = fromAccount.getBalance() - transferMoney;
				//set
				fromAccount.setBalance(fromBalance);
				
				//收款方增加
				double toBalance = toAccount.getBalance() + transferMoney;
				toAccount.setBalance(toBalance);
				
				//修改账户信息
				accountDao.updAccount(fromAccount);
				accountDao.updAccount(toAccount);
				System.out.println("转账成功");
				
				//提交事务
				DBUtils.commitTransaction();
				System.out.println("提交成功");
			}else{
				System.out.println("余额不足，转账失败");
			}
		}catch(SQLException e){
			System.out.println("提交失败，回滚。。。");
			//回滚事务
			DBUtils.roolback();
			e.printStackTrace();
		}catch(Exception e){
			e.printStackTrace();
		}finally{
			//关闭事务
			DBUtils.closeTransaction();
		}
	}
	
	
}
