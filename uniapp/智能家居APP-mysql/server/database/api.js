var connection = require('../database/sql.js')

//查询
const getAccount = () => {
  return new Promise((resolve, reject) => {
    //第一个参数：sql语句
    //第二个参数：回调函数（err：查询错误，data：查询结果）
    connection.query("select * from users",(err,data) => {
      resolve(data)
    })
  })
}
//添加
const insertAccount = (param) => {
  return new Promise((resolve,reject) => {
    connection.query("insert into users(username,password) values(?,?)",param,(err,data) => {
      //如果err为null则成功
      resolve(data)
    })
  })
}
//改
const updateThresholds = (param) => {
  return new Promise((resolve,reject) => {
    connection.query("update user_thresholds set temp_th = ? where username = ?",param,(err,data) => {
      //如果err为null则成功
      resolve(data)
    })
  })
}

//删除
// const deleteAccount = (param) => {
//   return new Promise((resolve,reject) => {
//     connection.query("delete from users where username = ?",param,(err,data) => {
//       resolve(data)
//     })
//   })
// }
//导出方法，在需要使用到的模块中导入
module.exports = {
  getAccount,
  insertAccount,
  updateThresholds,
  //deleteAccount
}
