package cn.xc.dao;

import java.sql.Connection;
import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.BeanHandler;

import cn.xc.entity.Account;
import cn.xc.util.DBUtils;

public class IAccountDaoImpl implements IAccountDao {

	@Override
	public Account queryAccountByName(String name) throws SQLException {
		// TODO Auto-generated method stub
		// 手动提交事务不需要传参
		QueryRunner runner = new QueryRunner();
		Connection ct = DBUtils.getConnection();
		String sql = "select * from account where name=?";

		// ???
		Object[] params = { name };

		Account account = null;
		// 查询账户
		// 异常交给上一层处理
		account = runner.query(ct, sql, new BeanHandler<Account>(Account.class), params);
		return account;
	}

	@Override
	public void updAccount(Account account) throws SQLException {
		// TODO Auto-generated method stub
		// 此时要传参？？？
		QueryRunner runner = new QueryRunner(DBUtils.getDataSourceWithC3P0ByXML());
		// 得到连接
		Connection ct = DBUtils.getConnection();
		String sql = "update account set balance=? where name=?";
		// 参数传值
		Object[] params = { account.getBalance(), account.getName() };
		runner.update(ct, sql, params);

	}

}
