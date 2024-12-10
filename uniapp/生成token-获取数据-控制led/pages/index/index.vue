<template>
	<view class="content">
		<view class="">温度 {{temp}} ℃</view>
		<view class="">湿度 {{humi}} %</view>
		<switch :checked="led" @change="onLedSwitch" />
	</view>
</template>

<script>
	const{createCommonToken} = require('@/key.js')  //导入函数
	export default {
		data() {
			return {
				token: '',
				temp: '',
				humi: '',
				led: false
			}
		},
		onLoad() {
			const params = {
				author_key : 'rQ9SmPK1mdPtwhRto/+8RNqy57QHHxqjd0GjEzLSkkiE4m2tBg3M43LRK+kvs7sW',
				version : '2022-05-01',
				user_id : '412104',
			}
			this.token = createCommonToken(params);
		},
		onShow(){
			//this.fetchDevData(); 
			// setInterval(()=>{//定时获取
			// 	this.fetchDevData(); 
			// },3000)
		},
		methods: {
			fetchDevData(){
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
					    console.log(res.data);

						this.humi = res.data.data[0].value;
						this.led = res.data.data[1].value === 'true'; //? true:false
						this.temp = res.data.data[2].value;
					}
				});
			},
			onLedSwitch(event){
				let value = event.detail.value; //获取设备端
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: { "led": value }
					},
					header: {
					    'authorization': this.token //自定义请求头信息
					},
					success: () => {
						
						console.log('LED '+ (value ? 'ON' : 'OFF'));
					}
				});
			}
			
		}
	}
</script>

<style>
	.content {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
	}

	.logo {
		height: 200rpx;
		width: 200rpx;
		margin-top: 200rpx;
		margin-left: auto;
		margin-right: auto;
		margin-bottom: 50rpx;
	}

	.text-area {
		display: flex;
		justify-content: center;
	}

	.title {
		font-size: 36rpx;
		color: #8f8f94;
	}
</style>
