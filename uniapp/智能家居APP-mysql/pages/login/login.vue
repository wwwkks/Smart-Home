<template>
	<view class="container">
		<view class="login-form">
			<input type="text" v-model="loginForm.username" placeholder="请输入用户名" />
			<input type="password" v-model="loginForm.password" placeholder="请输入密码" />
			<button @click="handleLogin">登录</button>
			<navigator url="/pages/register/register">没有账号？去注册</navigator>
		</view>
	</view>
</template>

<script>
	
	const user = require('@/common/common.js');
	export default {
		data() {
			return {
				loginForm: {
					username: '',
					password: ''
				}
			};
		},
		methods: {
			handleLogin() {
				// 这里需要发送请求到后端进行验证
				console.log('front-handleLogin')
				console.log(this.loginForm.username)
				console.log(this.loginForm.password)
				uni.request({
					url: 'http://localhost:3000/login', // 注意：这里的url应该是登录接口的URL
					method: 'POST', // 修改为POST方法
					data: {
						username: this.loginForm.username,
						password: this.loginForm.password
					},
					header: {
						'content-type': 'application/json' // 设置请求头，确保后端可以正确解析
					},
					success: (res) => {
						// 检查响应状态码和返回的数据
						if (res.statusCode === 200) {
							console.log(res)
							uni.showToast({
								title: '登录成功'
							});
							// 登录成功后的操作，比如保存用户信息，跳转到首页
							//uni.setStorageSync('userToken', res.data.token); // 假设后端返回了token
							user.setUser('username', this.loginForm.username);
							console.log('登录成功'+user.getUser('username'));
							uni.reLaunch({
								url:"/pages/index/index"
							})
						} else {
							uni.showToast({
								title: '登录失败',
								icon: 'none'
							});
						}
					},
					fail: (err) => {
						console.error(err);
						uni.showToast({
							title: '请求失败',
							icon: 'none'
						});
					}
				});
			}

		}
	};
</script>

<style>
	.container {
		display: flex;
		justify-content: center;
		align-items: center;
		height: 100vh;
	}

	.login-form {
		display: flex;
		flex-direction: column;
		width: 80%;
	}
</style>