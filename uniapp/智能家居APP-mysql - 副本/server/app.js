var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var bodyParser = require('body-parser');

var app = express(); // 创建 Express 应用实例

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// 跨域解决
app.all("*", function (req, res, next) {
  res.header("Access-Control-Allow-Origin", "*") // 允许所有域的请求
  res.header("Access-Control-Allow-Headers", "Content-Type") // 允许的请求头，这里只设置了Content-Type
  res.header("Access-Control-Allow-Methods", "*") // 允许所有的HTTP请求方法
  res.header("Content-Type", "application/json;charset=utf-8") // 设置响应的内容类型为JSON，编码为UTF-8
  next() // 继续处理请求
});

var indexRouter = require('./routes/index');
var usersRouter = require('./routes/users.js');

app.use('/', indexRouter);
app.use('/users', usersRouter);

// 注意：在 Express 4.16+ 版本中，已经内置了 bodyParser，所以下面的行是可选的
// 如果您使用的是 Express 4.16+，则不需要单独引入 bodyParser
app.use(bodyParser.urlencoded({extended: false}));
app.use(bodyParser.json()); // 用于解析 application/json

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

// app.listen(3000, () => {　　// 监听8033端口
//     console.log("3000启动中...");
// });
module.exports = app;
