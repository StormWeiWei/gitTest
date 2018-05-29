package cn.xc.util;

import java.sql.Connection;
import java.sql.SQLException;

import javax.sql.DataSource;

import com.mchange.v2.c3p0.ComboPooledDataSource;

public class DBUtils {

	// 获取本地事务
	// 定义THreadLocal对象，用于存放Connection对象
	public static ThreadLocal<Connection> threadLocal = new ThreadLocal<Connection>();

	// 获取数据源
	private static DataSource ds = new ComboPooledDataSource("kleine");

	// 获取c3p0数据源对象（从c3p0-config.xml中读取默认的数据库配置）
	public static DataSource getDataSource() {
		// 静态方法只能调用静态变量
		return ds;
	}

	// 通过C3P0配置文件方式，获取数据源对象
	// 通过C3P0配置文件方式，获取数据源对象
	public static DataSource getDataSourceWithC3P0ByXML() {
		//
		ComboPooledDataSource cpds = new ComboPooledDataSource("kleine");
		return cpds;
	}

	// 获取连接
	public static Connection getConnection() {
		Connection ct = threadLocal.get();
		try {
			if (ct == null) {
				ct = ds.getConnection();
			}
			// 获取连接后再set threadLocal
			threadLocal.set(ct);

		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return ct;
	}

	// 开启事务
	public static void beginTransaction() {
		// 得到(存在的)连接
		Connection ct = getConnection();
		try {
			// 手动开启事务
			ct.setAutoCommit(false);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 提交事务
	public static void commitTransaction() {
		// 得到副本连接
		Connection ct = threadLocal.get();
		try {
			if (ct != null) {
				ct.commit();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 回滚事务
	public static void roolback() {
		Connection ct = threadLocal.get();
		try {
			if (ct != null) {
				ct.rollback();// 回滚
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// 关闭连接
	public static void closeTransaction() {
		Connection ct = threadLocal.get();
		try {
			if (ct != null) {
				ct.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			// 从集合中移除当前绑定的连接
			threadLocal.remove();
			ct = null;
		}
	}

}
