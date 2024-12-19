var express = require('express');
var router = express.Router();
var connection = require('../database/sql.js')

/* GET users listing.   localhost:3000/users/users */  
// router.get("/users", function(req, res, next) {
//     connection.query('SELECT * from users', function(error, results, fields) {
//         if (error) throw error;
//         console.log('The solution is: ', results);
//         res.send(results)
//     });
// });
// // localhost:3000/users/user_thresholds
// router.get("/user_thresholds", function(req, res, next) {
//     connection.query('SELECT * from user_thresholds', function(error, results, fields) {
//         if (error) throw error;
//         console.log('The solution is: ', results);
//         res.send(results)
//     });
// });

module.exports = router;
