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

		</view>
		<view class="subsection">
			<view class="dev-name2">LED亮度</view><u-subsection activeColor="#13227a" :list="list1" :current="curNow1"
				@change="handleClick"></u-subsection>
		</view>

		<view class="subsection">
			<view class="dev-name2">报警器音量</view><u-subsection activeColor="#13227a" :list="list2" :current="curNow2"
				@change="handleBeepClick"></u-subsection>
		</view>

		<view class="subsection">
			<view class="dev-name2">风扇</view><u-subsection activeColor="#13227a" :list="list3" :current="curNow3"
				@change="handleSpeedClick"></u-subsection>
		</view>

		<view class="subsection">
			<view class="dev-name2">彩灯</view><u-subsection activeColor="#13227a" :list="list4" :current="curNow4"
				@change="handleModeClick"></u-subsection>
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
				gramode: 0,
				list1: ['暗', '较暗', '较亮', '亮'],
				list2: ['小', '较小', '中', '大'],
				list3: ['关', '慢', '中', '快'],
				list4: ['关', '模式一', '模式二', '模式三'],
				curNow1: 0,
				curNow2: 0,
				curNow3: 0,
				curNow4: 0
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
			sectionChange(index) {
				this.curNow = index;
			},


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
			handleClick(index) {
				this.curNow1 = index;

				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'bright': this.curNow1
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('亮度调整成功:' + this.curNow1);
					},
					fail: () => {
						console.log('亮度调整失败');
					}
				});
			},
			handleBeepClick(index) {
				this.curNow2 = index;

				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'sound': this.curNow2
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('报警器响度调整成功:' + this.curNow2);
					},
					fail: () => {
						console.log('报警器响度调整失败');
					}
				});
			},
			handleSpeedClick(index) {
				this.curNow3 = index;

				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'speed': this.curNow3
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('风扇速度调整成功:' + this.curNow3);
					},
					fail: () => {
						console.log('风扇速度调整失败');
					}
				});
			},
			handleModeClick(index) {
				this.curNow4 = index;

				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							'gramode': this.curNow4
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: () => {
						console.log('彩灯模式调整成功:' + this.curNow4);
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

	/* 父容器样式 */
	.dev-cart-long {
		height: 150rpx;
		width: 690rpx;
		border-radius: 30rpx;
		margin-top: 30px;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	/* 按钮样式 */
	.button {
		width: 150rpx;
		height: 60rpx;
		line-height: 60rpx;
		text-align: center;
		background-color: #13227a;
		color: #fff;
		border-radius: 10rpx;
		font-size: 16px;
		border: none;
		cursor: pointer;
		box-shadow: 0 0 15rpx #ccc;
		margin: 0 10rpx;
	}

	.dev-name {
		font-size: 20rpx;
		color: #6d6d6d;
		text-align: center;
	}
	
	.dev-name2 {
		font-size: 30rpx;
		color: black;
		text-align: center;
	}

	.dev-logo {
		height: 70rpx;
		width: 70rpx;
		margin-top: 10rpx;
	}

	.control-header {
		display: flex;
		align-items: center;
		margin-bottom: 20rpx;
	}

	.button-group {
		display: flex;
		justify-content: space-around;

		/deep/ .u-button {
			margin: 0 10rpx;
			min-width: 120rpx;
		}
	}

	.subsection {
		height: 100rpx;
		width: 690rpx;
		margin-top: 30px;
		flex-wrap: wrap;
		/* 自动换行 */
	}
</style>