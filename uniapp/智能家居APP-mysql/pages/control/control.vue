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
				// bright: 0,
				// sound: 0,
				// speed: 0,
				// gramode: 0,
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
			this.fetchData() ;
		},
		onShow() {
			setInterval(() => {
				this.fetchData() ;	
			}, 3000);
			setInterval(() => {

				this.updateState();
				
			}, 15000);

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
			updateState() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							// "led": this.led,
							// "beep": this.beep,
							"bright": this.curNow1,
							"sound": this.curNow2,
							"speed": this.curNow3,
							"gramode": this.curNow4,
						}
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: () => {
						console.log("当前状态发送成功");
					},
					fail: () => {
						console.log('当前状态发送失败')
					}
				});
			},
			fetchData() {
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
						this.led = res.data.data[5].value === 'true'; //? true:false
						console.log('led:'+ this.led);
						console.log('beep:' +this.beep);
					}
				});
			},

			onLedSwitch(event) {
				let value = event.detail.value;
				// 发送请求更新设备属性
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							"led": value
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					success: () => {
						console.log('LED ' + (value ? 'ON' : 'OFF') + ' !');
					}
				});
			},
			onBeepSwitch(event) {
				let value = event.detail.value;
				// 发送请求更新设备属性
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', // 仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							"beep": value
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					success: () => {
						console.log('BEEP ' + (value ? 'ON' : 'OFF') + ' !');
					}
				});
			},
			
			handleClick(index) {
				this.curNow1 = index;
				this.updateState();

			},
			handleBeepClick(index) {
				this.curNow2 = index;
				this.updateState();

			},
			handleSpeedClick(index) {
				this.curNow3 = index;
				this.updateState();

			},
			handleModeClick(index) {
				this.curNow4 = index;
				this.updateState();
			}

		}
	}
</script>

<style>
	
	.wrap {
	  padding: 30rpx;
	}
	
	.dev-area {
	  display: flex;
	  justify-content: space-between;
	  flex-wrap: wrap;
	}
	
	.dev-cart {
	  height: 150rpx;
	  width: 320rpx;
	  border-radius: 30rpx;
	  margin-top: 30px;
	  display: flex;
	  justify-content: space-around;
	  align-items: center;
	  box-shadow: 0 0 15rpx #ccc;
	}
	
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
	  color: #000;
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
	}
	
	.button-group .u-button {
	  margin: 0 10rpx;
	  min-width: 120rpx;
	}
	
	.subsection {
	  height: 100rpx;
	  width: 690rpx;
	  margin-top: 30px;
	  flex-wrap: wrap;
	}


</style>