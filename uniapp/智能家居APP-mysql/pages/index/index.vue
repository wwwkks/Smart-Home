<template>

	<view class="wrap">
		<view>
			<u-notice-bar :text="computedText"></u-notice-bar>

			<!-- 测试按钮，用于模拟条件切换 -->
			<!-- 			<view style="margin-top: 20px;">
				<button @click="toggleCondition('condition1')">切换条件1</button>
				<button @click="toggleCondition('condition2')">切换条件2</button>
				<button @click="toggleCondition('condition3')">切换条件3</button>
			</view> -->
		</view>
		<!-- 测试-->
		<!-- <button @click="showPopup">显示弹出框</button> -->
		<!-- 引入的全局弹出框组件 -->
		<GlobalPopup ref="popup" />

		<view class="weather-card">
			<view>威海</view>
			<view class="weather-info">
				<view class="weather-temp">{{ weather.temp !== undefined ? weather.temp : '加载中...' }} ℃</view>
				<view class="weather-text">{{ weather.text !== undefined ? weather.text : '加载中...' }}</view>
				<view class="weather-humidity">湿度: {{ weather.humi !== undefined ? weather.humi : '加载中...' }}%</view>
				<view class="weather-wind">风速: {{ weather.windspeed !== undefined ? weather.windspeed : '加载中...' }} km/h</view>
				<view class="weather-wind">空气{{ air.category !== undefined ? air.category : '加载中...' }} {{ air.aqi !== undefined ? air.aqi : '加载中...' }}</view>
				<view class="weather-wind">{{ suggest !== undefined ? suggest : '加载中...' }}</view>
				<view class="weather-wind">预警信息: {{warning !== undefined ? warning : '加载中...' }}</view>
			</view>
		</view>

		<view class="dev-area">
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">温度</view>
					<image class="dev-logo" src="../../static/temp.png" mode=""></image>
				</view>
				<view class="dev-data">{{ temp }} ℃</view>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">湿度</view>
					<image class="dev-logo" src="../../static/humi.png" mode=""></image>
				</view>
				<view class="dev-data">{{ humi }} %</view>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">光照</view>
					<image class="dev-logo" src="../../static/lsens.png" mode=""></image>
				</view>
				<view class="dev-data">{{ lsens }} lx</view>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">烟雾</view>
					<image class="dev-logo" src="../../static/ppm.png" mode=""></image>
				</view>
				<view class="dev-data">{{ ppm }} ppm</view>
			</view>

		</view>
	</view>
</template>

<script>
	import GlobalPopup from '@/components/GlobalPopup.vue'
	const user = require('@/common.js')
	import {
		ip
	} from '@/common.js'
	const {
		createCommonToken
	} = require('@/key.js') //导入函数
	import {
		parse,
		stringify
	} from 'querystring';


	export default {
		data() {
			return {
				token: '',
				temp: '',
				humi: '',
				lsens: '',
				ppm: '',
				led: false,
				beep: false,
				bright: '',
				weather_temp: 0,
				weather: {
					temp: '4',
					humi: '40',
					windspeed: '34',
					text: '多云'
				},
				air: {
					category: '优',
					aqi: '24'
				},
				warning: '',
				suggest: null,
				temp_th: 100,
				humi_th: 100,
				lsens_th: 100,
				ppm_th: 100,
				text1: "警告：温度超出阈值!!!",
				text2: "警告：湿度超出阈值!!!",
				text3: "警告：光照强度超出阈值!!!",
				text4: "警告：烟雾报警!!!",
				condition1: false,
				condition2: false,
				condition3: false,
				condition4: false
			}
		},
		computed: {
			// 根据条件动态生成通知文本
			computedText() {
				let result = "";
				if (this.temp > this.temp_th) {
					result += this.text1;
				}
				if (this.humi > this.humi_th) {
					result += this.text2;
				}
				if (this.lsens > this.lsens_th) {
					result += this.text3;
				}
				if (this.ppm > this.ppm_th) {
					result += this.text4;
				}
				// 默认内容（当没有条件满足时）
				if (this.temp <= this.temp_th && this.humi <= this.humi_th && this.lsens <= this.lsens_th && this.ppm <=
					this.ppm_th) {
					result = "温度,湿度,光照,烟雾传感器检测正常";
				}
				return result;
			}
		},
		onLoad() {
			const params = {
				author_key: 'rQ9SmPK1mdPtwhRto/+8RNqy57QHHxqjd0GjEzLSkkiE4m2tBg3M43LRK+kvs7sW',
				version: '2022-05-01',
				user_id: '412104',
			}
			this.token = createCommonToken(params);
			this.fetchDevData();
			this.getWeather();
			this.showPopup();
		},
		onShow() {
			this.fetchDevData();
			this.showPopup();
			setInterval(() => { //定时获取
				this.fetchDevData();
			}, 6000);
			setInterval(() => {
				this.showPopup();
			}, 10000);
		},
		onNavigationBarButtonTap: function(e) {
			uni.reLaunch({
				url: '/pages/login/login'
			})
		},
		methods: {
			// 切换条件
			toggleCondition(condition) {
				this[condition] = !this[condition];
			},
			fetchThreshold() {
				console.log("阈值check");
				console.log(user.getAll())
				console.log(ip)
				uni.request({
					//url: "http://192.168.198.1:3000/get_thresholds",
					url: `http://${ip}/get_thresholds`,
					method: 'GET',
					data: {
						username: user.getUser('username') // 替换为实际的用户名
					},
					success: res => {
						if (res.statusCode === 200) {
							user.setUser('temp_th', res.data.temp_th);
							user.setUser('humi_th', res.data.humi_th);
							user.setUser('lsens_th', res.data.lsens_th);
							user.setUser('ppm_th', res.data.ppm_th);
							this.temp_th = user.getUser('temp_th');
							this.humi_th = user.getUser('humi_th');
							this.lsens_th = user.getUser('lsens_th');
							this.ppm_th = user.getUser('ppm_th');
						} else {
							console.error('Failed to get thresholds:', res.data.error);
						}
					},
					fail: (err) => {
						console.error('Request failed:', err);
					},
				});
			},

			showPopup() {
				this.fetchThreshold();

				if (this.temp > this.temp_th) {
					console.log("弹窗temp:" + this.temp + "---------------" + this.temp_th)
					//this.toggleCondition('condition1');
					this.$refs.popup.showPopup();
				}

				if (this.humi > this.humi_th) {
					console.log("弹窗humi:" + this.humi + "---------------" + this.humi_th)
					//this.toggleCondition('condition2')
					this.$refs.popup.showPopup2()
				}


				if (this.lsens > this.lsens_th) {
					console.log("弹窗lsens:" + this.lsens + "---------------" + this.lsens_th)
					//this.toggleCondition('condition3')
					this.$refs.popup.showPopup3();
				}

				if (this.ppm > this.ppm_th) {
					console.log("弹窗ppm:" + this.ppm + "---------------" + this.ppm_th)
					//this.toggleCondition('condition4')
					this.$refs.popup.showPopup4();
				}
			},
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', //仅为示例，并非真实接口地址。
					method: 'GET',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1'
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: (res) => {
						console.log('获取设备数据');
						console.log(res.data);
						this.beep = res.data.data[0].value === 'true';
						this.humi = res.data.data[3].value;
						this.led = res.data.data[4].value === 'true'; //? true:false
						this.temp = res.data.data[12].value;
						this.lsens = res.data.data[6].value;
						this.ppm = res.data.data[8].value;
					}
				});
			},
			getWeather() {
				console.log("天气API");

				const weatherRequest = new Promise((resolve, reject) => {
					uni.request({
						url: 'https://devapi.qweather.com/v7/weather/now?location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
						method: 'GET',
						success: (res) => {
							console.log("天气请求成功")
							console.log(res)
							if (res.data.code === "200") {
								this.weather.temp = res.data.now.temp;
								// this.weather.humi = res.data.now.humidity;
								 this.$set(this.weather, 'humi', res.data.now.humidity);
								this.weather.windspeed = res.data.now.windSpeed;
								this.weather.text = res.data.now.text;
								resolve();
							} else {
								console.error("Error fetching weather data: ", res.data);
								reject(res.data);
							}
						},
						fail: (error) => {
							console.error("Error fetching weather data: ", error);
							reject(error);
						}
					});
				});

				const airRequest = new Promise((resolve, reject) => {
					uni.request({
						url: 'https://devapi.qweather.com/v7/air/now?location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
						method: 'GET',
						success: (res) => {
							if (res.data.code === "200") {
								this.air.category = res.data.now.category;
								this.air.aqi = res.data.now.aqi;
								resolve();
							} else {
								console.error("Error fetching air data: ", res.data);
								reject(res.data);
							}
						},
						fail: (error) => {
							console.error("Error fetching air data: ", error);
							reject(error);
						}
					});
				});

				const warningRequest = new Promise((resolve, reject) => {
					uni.request({
						url: 'https://devapi.qweather.com/v7/warning/now?location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
						method: 'GET',
						success: (res) => {
							if (res.data.code === "200") {
								this.warning = res.data.warning[0] || '无'; // 存储天气数据
								resolve();
							} else {
								console.error("Error fetching warning data: ", res.data);
								reject(res.data);
							}
						},
						fail: (error) => {
							console.error("Error fetching warning data: ", error);
							reject(error);
						}
					});
				});

				const suggestRequest = new Promise((resolve, reject) => {
					uni.request({
						url: 'https://devapi.qweather.com/v7/indices/1d?type=8&location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
						method: 'GET',
						success: (res) => {
							if (res.data.code === "200") {
								this.suggest = res.data.daily[0].text; // 存储天气数据
								resolve();
							} else {
								console.error("Error fetching suggest data: ", res.data);
								reject(res.data);
							}
						},
						fail: (error) => {
							console.error("Error fetching suggest data: ", error);
							reject(error);
						}
					});
				});

				Promise.all([weatherRequest, airRequest, warningRequest, suggestRequest])
					.then(() => {
						console.log(this.weather.temp);
						console.log(this.air.aqi);
						console.log(this.suggest);
					})
					.catch((error) => {
						console.error("One or more requests failed: ", error);
					});
			}

		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
		/* 内边距设置 */
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		/* 排列 */
		flex-wrap: wrap;
		/* 自动换行 */
	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx;
		/* 弧度 */
		margin-top: 30px;
		display: flex;
		justify-content: space-around;
		/* 内部内容 */
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
		/* 边框 */
	}

	.dev-cart-long {
		height: 150rpx;
		width: 690rpx;
		border-radius: 30rpx;
		/* 弧度 */
		margin-top: 30px;
		display: flex;
		justify-content: space-around;
		/* 内部内容 */
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
		/* 边框 */
	}

	.dev-name {
		font-size: 20rpx;
		color: #6d6d6d;
		text-align: center;
	}

	.dev-logo {
		height: 70rpx;
		width: 70rpx;
		margin-top: 10rpx;
	}

	.dev-data {
		font-size: 50rpx;
		color: #6d6d6d;
	}


	.weather-card {
		padding: 30rpx;
		/* 增加内边距 */
		border-radius: 15rpx;
		/* 调整圆角 */
		box-shadow: 0 4rpx 20rpx rgba(0, 0, 0, 0.1);
		/* 更柔和的阴影 */
		margin-top: 20rpx;
		background-color: #f0f8ff;
		/* 更柔和的背景色 */
		text-align: center;
		/* 文字居中 */
	}

	.weather-info {
		margin-top: 10rpx;
		/* 增加顶部间距 */
	}

	.weather-temp {
		font-size: 50rpx;
		/* 增加字体大小 */
		font-weight: bold;
		color: #333;
		/* 更深的颜色 */
	}

	.weather-text {
		font-size: 24rpx;
		/* 增加字体大小 */
		color: #666;
		/* 更柔和的颜色 */
	}

	.weather-humidity,
	.weather-wind {
		font-size: 18rpx;
		/* 增加字体大小 */
		color: #888;
		/* 更柔和的颜色 */
		margin: 5rpx 0;
		/* 增加上下间距 */
	}

	.weather-warning {
		font-size: 16rpx;
		/* 增加字体大小 */
		color: #d9534f;
		/* 警告信息颜色 */
		margin-top: 10rpx;
		/* 增加顶部间距 */
	}
</style>