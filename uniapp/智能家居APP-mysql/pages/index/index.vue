<template>

	<view class="wrap">
		<view class="dev-area">

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">温度</view>
					<image class="dev-logo" src="../../static/temp.png" mode=""></image>
				</view>
				<view class="dev-data">{{temp}} ℃</view>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">湿度</view>
					<image class="dev-logo" src="../../static/humi.png" mode=""></image>
				</view>
				<view class="dev-data">{{humi}} %</view>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">光照</view>
					<image class="dev-logo" src="../../static/lsens.png" mode=""></image>
				</view>
				<view class="dev-data">{{lsens}} lx</view>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">烟雾</view>
					<image class="dev-logo" src="../../static/ppm.png" mode=""></image>
				</view>
				<view class="dev-data">{{ppm}} ppm</view>
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
				// username:'',
				// password:''
			}
		},
		onLoad() {
			const params = {
				author_key: 'rQ9SmPK1mdPtwhRto/+8RNqy57QHHxqjd0GjEzLSkkiE4m2tBg3M43LRK+kvs7sW',
				version: '2022-05-01',
				user_id: '412104',
			}
			this.token = createCommonToken(params);
		},
		onShow() {
			setInterval(() => { //定时获取
				this.fetchDevData();
			}, 3000)
		},
		onNavigationBarButtonTap: function(e) {
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
						this.humi = res.data.data[2].value;
						this.led = res.data.data[4].value === 'true'; //? true:false
						this.temp = res.data.data[9].value;
						this.lsens = res.data.data[5].value;
						this.ppm = res.data.data[7].value;
					}
				});
			}
		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx; //内边距设置
	}

	.dev-area {
		display: flex;
		justify-content: space-between; //排列
		flex-wrap: wrap; //自动换行
	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx; //弧度
		margin-top: 30px;
		display: flex;
		justify-content: space-around; //内部内容
		align-items: center;
		box-shadow: 0 0 15rpx #ccc; //边框
	}

	.dev-cart-long {
		height: 150rpx;
		width: 690rpx;
		border-radius: 30rpx; //弧度
		margin-top: 30px;
		display: flex;
		justify-content: space-around; //内部内容
		align-items: center;
		box-shadow: 0 0 15rpx #ccc; //边框
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

	/* 长设备卡片样式 */
	.device-cart-l {
		height: 150rpx;
		/* 设置高度为150像素 */
		width: 700rpx;
		/* 设置宽度为700像素 */
		border-radius: 30rpx;
		/* 设置边框圆角为30像素 */
		margin-top: 30rpx;
		/* 设置上外边距为30像素 */
		display: flex;
		/* 使用弹性盒子布局 */
		justify-content: space-around;
		/* 在弹性容器中均匀分布子元素，两端对齐 */
		align-items: center;
		/* 在弹性容器中垂直居中对齐子元素 */
		box-shadow: 0 0 15rpx #ccc;
		/* 设置盒子阴影，颜色为灰色 */
	}


	/* 滑动条样式 */
	.ctrl-slider {
		width: 580rpx;
		/* 设置宽度为580像素 */
	}
</style>