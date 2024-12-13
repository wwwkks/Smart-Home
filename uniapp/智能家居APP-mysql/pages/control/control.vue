<template>

	<view class="wrap">
		<view class="dev-area">

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">台灯</view>
					<image class="dev-logo" src="../../static/led.png" mode=""></image>
				</view>
				<switch :checked="led" @change="onLedSwitch" color="#13227a" />
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">报警器</view>
					<image class="dev-logo" src="../../static/beep.png" mode=""></image>
				</view>
				<switch :checked="beep" @change="onBeepSwitch" color="#13227a" />
			</view>

			<view class="dev-cart-long">
				<view class="dev-name">亮度</view>
				<button class="mini-btn" type="default" @click="handleClick('button1')">暗</button>
				<button class="mini-btn" type="default" @click="handleClick('button2')">较暗</button>
				<button class="mini-btn" type="default" @click="handleClick('button3')">较亮</button>
				<button class="mini-btn" type="default" @click="handleClick('button4')">亮</button>
			</view>
			
			<view class="dev-cart-long">
				<view class="dev-name">响度</view>
				<button class="mini-btn" type="default" @click="handleBeepClick('button1')">小</button>
				<button class="mini-btn" type="default" @click="handleBeepClick('button2')">较小</button>
				<button class="mini-btn" type="default" @click="handleBeepClick('button3')">较大</button>
				<button class="mini-btn" type="default" @click="handleBeepClick('button4')">大</button>
			</view>
			
			<view class="dev-cart-long">
				<view class="dev-name">转速</view>
				<button class="mini-btn" type="default" @click="handleSpeedClick('button1')">关</button>
				<button class="mini-btn" type="default" @click="handleSpeedClick('button2')">慢档</button>
				<button class="mini-btn" type="default" @click="handleSpeedClick('button3')">中档</button>
				<button class="mini-btn" type="default" @click="handleSpeedClick('button4')">快档</button>
			</view>
			
			<view class="dev-cart-long">
				<view class="dev-name">彩灯</view>
				<button class="mini-btn" type="default" @click="handleModeClick('button1')">关</button>
				<button class="mini-btn" type="default" @click="handleModeClick('button2')">模式一</button>
				<button class="mini-btn" type="default" @click="handleModeClick('button3')">模式二</button>
				<button class="mini-btn" type="default" @click="handleModeClick('button4')">模式三</button>
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
				led: false,
				beep: false,
				bright: 0,
				sound: 0,
				speed: 0,
				gramode: 0 
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
		onNavigationBarButtonTap: function(e) {
			uni.reLaunch({
				url: '/pages/login/login'
			})
		},
		methods: {
			onLedSwitch(event) {
				let value = event.detail.value; //获取设备端
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							"led": value
						}
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: () => {
						console.log('LED ' + (value ? 'ON' : 'OFF'));
					},
					fail: () => {
						console.log('LED fail')
					}
				});
			},
			onBeepSwitch(event) {
				let value = event.detail.value; //获取设备端
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							"beep": value
						}
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: () => {
						console.log('BEEP ' + (value ? 'ON' : 'OFF'));
					}
				});
			},
			handleClick(id) {
				console.log(id)
				if (id == 'button1') {
					this.bright = 1
				} else if (id == 'button2') {
					this.bright = 2
				} else if (id == 'button3') {
					this.bright = 3
				} else if (id == 'button4') {
					this.bright = 4
				}

				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'bright': this.bright
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('亮度调整成功:' + this.bright);
					},
					fail: () => {
						console.log('亮度调整失败');
					}
				});
			},
			handleBeepClick(id) {
				console.log(id)
				if (id == 'button1') {
					this.sound = 1
				} else if (id == 'button2') {
					this.sound = 2
				} else if (id == 'button3') {
					this.sound = 3
				} else if (id == 'button4') {
					this.sound = 4
				}
			
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'sound': this.sound
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('报警器响度调整成功:' + this.sound);
					},
					fail: () => {
						console.log('报警器响度调整失败');
					}
				});
			},
			handleSpeedClick(id) {
				console.log(id)
				if (id == 'button1') {
					this.speed = 0
				} else if (id == 'button2') {
					this.speed= 1
				} else if (id == 'button3') {
					this.speed = 2
				} else if (id == 'button4') {
					this.speed = 3
				}
			
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'speed': this.speed
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('风扇速度调整成功:' + this.speed);
					},
					fail: () => {
						console.log('风扇速度调整失败');
					}
				});
			},
			handleModeClick(id) {
				console.log(id)
				if (id == 'button1') {
					this.gramode = 0
				} else if (id == 'button2') {
					this.gramode = 1
				} else if (id == 'button3') {
					this.gramode = 2
				} else if (id == 'button4') {
					this.gramode = 3
				}
			
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'gramode': this.gramode
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('彩灯模式调整成功:' +this.gramode);
					},
					fail: () => {
						console.log('彩灯模式调整失败');
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