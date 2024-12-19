<template>
	<view class="container">
		<view class="login-form">
			<view class="login-title">登录</view>
			<input type="text" v-model="loginForm.username" placeholder="请输入用户名" />
			<input type="password" v-model="loginForm.password" placeholder="请输入密码" />
			<button @click="handleLogin">登录</button>
			<navigator url="/pages/register/register">没有账号？去注册</navigator>
		</view>
	</view>
</template>

<script>
import store from '@/store/index.js';//需要引入store
import {ip} from '@/common.js';
const user = require('@/common.js');
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
			console.log('请求登录中...')
			console.log(ip);
			console.log(this.loginForm.username)
			console.log(this.loginForm.password)
			uni.request({
				//url: 'http://172.27.2.244:3000/login', // 注意：这里的url应该是登录接口的URL
				url: `http://${ip}/login`, // 使用模板字符串
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
						// 登录成功后保存用户信息到 vuex
						this.$store.commit('SET_USER_INFO', {
							username: this.loginForm.username
						})
						console.log(res)
						uni.showToast({
							title: '登录成功'
						});
						// 登录成功后的操作，比如保存用户信息，跳转到首页
						//uni.setStorageSync('userToken', res.data.token); // 假设后端返回了token
						user.setUser('username', this.loginForm.username);
						console.log('登录成功' + user.getUser('username'));
						uni.reLaunch({
							url: "/pages/index/index"
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
	background: linear-gradient(135deg, #f5f7fa, #c3cfe2);
	/* 添加渐变背景 */
}

.login-form {
	display: flex;
	flex-direction: column;
	width: 80%;
	padding: 30px;
	background-color: #ffffff;
	border-radius: 8px;
	box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
}

.login-form input {
	height: 40px;
	margin-bottom: 20px;
	padding: 0 15px;
	border: 1px solid #dcdfe6;
	border-radius: 4px;
	font-size: 14px;
}

.login-form input:focus {
	border-color: #409eff;
	outline: none;
	box-shadow: 0 0 5px rgba(64, 158, 255, 0.5);
	/* 添加聚焦阴影 */
}

.login-form button {
	height: 40px;
	background-color: #409eff;
	color: #ffffff;
	border: none;
	border-radius: 4px;
	font-size: 16px;
	cursor: pointer;
	transition: background-color 0.3s;
}

.login-form button:hover {
	background-color: #66b1ff;
	/* 悬停时改变背景色 */
	transform: translateY(-2px);
	/* 悬停时轻微上移 */
}

.login-form button:active {
	background-color: #3a8ee6;
	transform: translateY(0);
	/* 点击时恢复位置 */
}

.login-title {
	color: #409eff;
	font-size: 24px;
	text-align: center;
	margin-bottom: 30px;
}
</style>