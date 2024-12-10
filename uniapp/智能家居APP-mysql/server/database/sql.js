var mysql = require('mysql');
const express = require("express");　　// 引入express模块
const app = express();　　// 实例化express模块 用app来表示

var connection = mysql.createConnection({
    host: 'localhost', //host地址
    port:3306, //端口号
    user: 'root', //连接数据库时的账号
    password: '123456',//连接数据库时的密码
    database: 'smart-home' //需要连接的数据库
});

connection.connect(); // 与数据库建立连接

app.listen(8033, () => {　　// 监听8033端口
    console.log("8033启动中...");
});
module.exports = connection;