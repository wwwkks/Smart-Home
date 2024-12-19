// router.get("/get_account",(req,res,next) => {
//   getAccount()
//   .then(response => {
//     res.send(response)
//   })
// });

// router.post("/insert_account",(req,res,next) => {
//   let param = req.body
//   let {username,password} = param
//   insertAccount([username,password])
//   .then(response => {
//     res.send(response)
//   })
// });

// router.post("/update_thresholds",(req,res,next) => {
//   let param = req.body
//   let {username,temp_th} = param
//   updateThresholds([username,temp_th])
//   .then(response => {
//     res.send(response)
//   })
// });



// router.get("/delete_account",(req,res,next) => {
//   let id = req.query.id
//   deleteAccount([id])
//   .then(response => {
//     res.send(response)
//   })
// })

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

// router.post("/home",(req,res) => {
//   console.log(req.body)
//   res.send(req.body)
// })

// router.get("/users/:id/:name",(req,res) => {
//   console.log(req.params)// {"id": "123","name": "tom"}
//   res.send(req.params)
// })