var express = require('express');
var router = express.Router();
var connection = require('../database/sql.js')
const bcrypt = require('bcryptjs');
//const {getAccount,insertAccount,updateThresholds} = require('../database/api')  //deleteAccount

router.get('/', function(req, res, next) {
	res.render('index', {
		title: 'Express'
	});
});

// 注册路由
router.post('/register', (req, res) => {
	const {
		username,
		password
	} = req.body;
	// 密码加密
	bcrypt.hash(password, 10, (err, hash) => {
		if (err) {
			return res.status(500).send({
				message: err
			});
		}
		// 查询数据库，检查用户名是否已存在
		const sql = 'SELECT * FROM users WHERE username = ?';
		connection.query(sql, [username], (err, results) => {
			if (err) {
				return res.status(500).send({
					message: err
				});
			}
			if (results.length > 0) {
				return res.status(400).send({
					message: 'Username already exists'
				});
			}
			// 插入新用户
			const insertSql = 'INSERT INTO users (username, password) VALUES (?, ?)';
			connection.query(insertSql, [username, hash], (err, results) => {
				if (err) {
					return res.status(500).send({
						message: err
					});
				}
				res.status(201).send({
					message: 'User registered successfully'
				});
			});
		});
	});
});

router.post('/login', (req, res) => {
	console.log('login-post')
	console.log(req)
	
	const {
		username,
		password
	} = req.body;
	
	console.log(username)
	console.log(password)
	// 查询数据库，检查用户名是否存在
	const sql = 'SELECT * FROM users WHERE username = ?';
	connection.query(sql, [username], (err, results) => {
		if (err) {
			return res.status(500).send({
				message: err
			});
		}
		if (results.length === 0) {
			return res.status(400).send({
				message: 'Username not found'
			});
		}
		// 验证密码
		bcrypt.compare(password, results[0].password, (err, match) => {
			if (err) {
				return res.status(500).send({
					message: err
				});
			}
			if (!match) {
				return res.status(400).send({
					message: 'Incorrect password'
				});
			}
			res.status(200).send({
				message: 'Login successful'
			});
		});
	});
});

//阈值获取
router.get('/get_thresholds', function(req, res) {
	console.log('get_thresholds请求');
    var sql = 'SELECT temp_th, humi_th, lsens_th, ppm_th FROM user_thresholds WHERE username = ?';
    var params = [req.query.username]; // 只需要传递username参数

    connection.query(sql, params, function(err, result) {
        if (err) {
            console.error("查询失败: " + err.stack);
            res.status(500).send({ error: "查询失败" });
            return;
        }
        if (result.length === 0) {
            // 如果没有找到对应的记录，可以返回一个错误或者空对象
            res.status(404).send({ error: "未找到对应的阈值设置" });
        } else {
            res.status(200).send(result[0]);
        }
    });
});

//阈值修改
router.put('/update_thresholds', function(req, res) {
    var sql = 'UPDATE user_thresholds SET temp_th=?, humi_th=?, lsens_th=?, ppm_th=? WHERE username=?';
    var params = [req.body.temp_th, req.body.humi_th, req.body.lsens_th, req.body.ppm_th, req.body.username];
    connection.query(sql, params, function(err, results) {
        if (err) {
            console.error("阈值修改失败: " + err.stack);
            res.status(500).send({ error: "阈值修改失败" });
            return;
        } else {
            res.status(200).send({ success: "阈值更新成功" });
        }
    });
});


router.get('/get_users', function(req, res) {
	var sql = 'select * from users'
	connection.query(sql, function(err, result) { //连接数据库 传入sql语句 回调函数得到结果
		try {
			res.send(result) //查询结果响应给请求方
		} catch (err) {
			console.log("查询失败");
		}
	});
})

router.post('/add_user', function(req, res) {
	var sql = 'insert into users(username,password) values(?,?)'; // 这边的"?"是SQL的模板语法
	var params = [req.query.username, req.query.password] // 这边的数组参数与上边的"?"一一映射
	connection.query(sql, params, function(err, result) {
		try {
			res.send('增加数据成功');
		} catch (err) {
			console.log('新增数据失败');
		}
	});
})

module.exports = router;