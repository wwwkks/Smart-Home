var mysql = require('mysql');
const express = require("express");　　// 引入express模块


var connection = mysql.createConnection({
    host: '192.168.109.114', //host地址     localhost   172.26.234.135  192.168.109.114
    port:3306, //端口号
    user: 'root', //连接数据库时的账号
    password: '123456',//连接数据库时的密码
    database: 'smart-home' //需要连接的数据库
});

connection.connect(); // 与数据库建立连接

module.exports = connection;