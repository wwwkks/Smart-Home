<template>

	<view class="wrap">
		<view class="weather-card">
			<view>威海</view>
			<view class="weather-info">
				<view class="weather-temp">{{ weather.now.temp }} ℃</view>
				<view class="weather-text">{{ weather.now.text }}</view>
				<view class="weather-humidity">湿度: {{ weather.now.humidity }}%</view>
				<view class="weather-wind">风速: {{ weather.now.windSpeed }} km/h</view>
				<view class="weather-wind">空气{{ air.now.category }} {{ air.now.aqi }}</view>
				<view class="weather-wind">{{ this.suggest }}</view>
				<view v-if="warning.warning.length > 0" class="weather-wind">预警信息: {{ warning.warning[0].title }}
				</view>
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
const {
	createCommonToken
} = require('@/key.js') //导入函数
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
			weather: null,
			air: null,
			warning: null,
			suggest: ''

		}
	},
	onLoad() {
		const params = {
			author_key: 'rQ9SmPK1mdPtwhRto/+8RNqy57QHHxqjd0GjEzLSkkiE4m2tBg3M43LRK+kvs7sW',
			version: '2022-05-01',
			user_id: '412104',
		}
		this.token = createCommonToken(params);
		this.getWeather();
	},
	onShow() {
		setInterval(() => { //定时获取
			this.fetchDevData();
		}, 3000)
	},
	onNavigationBarButtonTap: function (e) {
		uni.reLaunch({
			url: '/pages/login/login'
		})
	},
	methods: {
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
			uni.request({
				url: 'https://devapi.qweather.com/v7/weather/now?location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
				method: 'GET',
				success: (res) => {
					if (res.data.code === "200") {
						this.weather = res.data; // 存储天气数据
					} else {
						console.error("Error fetching weather data: ", res.data);
					}
				},
				fail: (error) => {
					console.error("Error fetching weather data: ", error);
				}
			});

			uni.request({
				url: 'https://devapi.qweather.com/v7/air/now?location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
				method: 'GET',
				success: (res) => {
					if (res.data.code === "200") {
						this.air = res.data; // 存储天气数据
					} else {
						console.error("Error fetching air data: ", res.data);
					}
				},
				fail: (error) => {
					console.error("Error fetching air data: ", error);
				}
			});

			uni.request({
				url: 'https://devapi.qweather.com/v7/warning/now?location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
				method: 'GET',
				success: (res) => {
					if (res.data.code === "200") {
						this.warning = res.data; // 存储天气数据
					} else {
						console.error("Error fetching warning data: ", res.data);
					}
				},
				fail: (error) => {
					console.error("Error fetching warning data: ", error);
				}
			});

			uni.request({
				url: 'https://devapi.qweather.com/v7/indices/1d?type=8&location=101121301&key=6ff97dc76f834ab2b0dd2e13b35c5775',
				method: 'GET',
				success: (res) => {
					if (res.data.code === "200") {
						this.suggest = res.data.daily[0].text; // 存储天气数据
					} else {
						console.error("Error fetching suggest data: ", res.data);
					}
				},
				fail: (error) => {
					console.error("Error fetching suggest data: ", error);
				}
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