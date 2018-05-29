package cn.xc.dao;

import java.sql.SQLException;

import cn.xc.entity.Account;

public interface IAccountDao {

	// 根据姓名查询账户
	public abstract Account queryAccountByName(String name) 
			throws SQLException;

	// 更新账户(增加余额，减少余额)
	public abstract void updAccount(Account account) 
			throws SQLException;

}
