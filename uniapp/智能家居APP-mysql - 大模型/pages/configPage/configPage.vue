<template>
	<view class="wrap">
		<view class="dev-area">
			<!-- 温度阈值长卡片 -->
			<view class="device-cart-l">
				<view>
					<view class="dev-name">温度阈值</view>
					<!-- 当前温度阈值显示 -->
					<view class="dev-name">{{temp_th}}℃</view>
				</view>
				<view class="ctrl-slider">
					<!-- 滑动条调用slider组件，实时更新阈值 -->
					<slider activeColor="#13227a" :value="temp_th" @change="sliderChange($event, 'slider1')" min="0" max="100" step="1"
						block-size="20" show-value />
				</view>
			</view>

			<!-- 湿度阈值长卡片 -->
			<view class="device-cart-l">
				<view>
					<view class="dev-name">湿度阈值</view>
					<!-- 当前湿度阈值显示 -->
					<view class="dev-name">{{humi_th}}%</view>
				</view>
				<view class="ctrl-slider">
					<!-- 滑动条调用slider组件，实时更新阈值 -->
					<slider  activeColor="#13227a" :value="humi_th" @change="sliderChange($event, 'slider2')" min="0" max="100" step="1"
						block-size="20" show-value />
				</view>
			</view>

			<view class="device-cart-l">
				<view>
					<view class="dev-name">光照阈值</view>
					<!-- 当前光照阈值显示 -->
					<view class="dev-name">{{lsens_th}}lx</view>
				</view>
				<view class="ctrl-slider">
					<!-- 滑动条调用slider组件，实时更新阈值 -->
					<slider activeColor="#13227a" :value="lsens_th" @change="sliderChange($event, 'slider3')" min="0" max="100" step="1"
						block-size="20" show-value />
				</view>
			</view>

			<view class="device-cart-l">
				<view>
					<view class="dev-name">可燃性气体阈值</view>
					<!-- 当前可燃性气体阈值显示 -->
					<view class="dev-name">{{ppm_th}}ppm</view>
				</view>
				<view class="ctrl-slider">
					<!-- 滑动条调用slider组件，实时更新阈值 -->
					<slider activeColor="#13227a" :value="ppm_th" @change="sliderChange($event, 'slider4')" min="0" max="100" step="1"
						block-size="20" show-value />
				</view>
			</view>
		</view>
	</view>
</template>

<script>
	import {ip} from '@/common.js';
	import {
		stringify
	} from 'querystring';
	import {fetchData} from '@/pages/control/control.vue'
	const {
		createCommonToken
	} = require('@/key.js') //导入函数
	const user = require('@/common.js')
	export default {
		data() {
			return {
				token: '',
				temp_th: 100,
				humi_th: 100,
				lsens_th: 100,
				ppm_th: 100,
			}
		},
		onShow() {
			// setInterval(() => { //定时发送
			// 	this.postThresholds();
			// }, 10000)
		},
		onLoad() {
			const params = {
				author_key: 'rQ9SmPK1mdPtwhRto/+8RNqy57QHHxqjd0GjEzLSkkiE4m2tBg3M43LRK+kvs7sW',
				version: '2022-05-01',
				user_id: '412104',
			}
			this.token = createCommonToken(params);
			
			uni.request({
				//url: "http://localhost:3000/get_thresholds",
				url: `http://${ip}/get_thresholds`, 
				method: 'GET',
				data: {
					username: user.getUser('username') // 替换为实际的用户名
				},
				success: res => {
					if (res.statusCode === 200) {
						// 假设后端返回的是阈值对象
						console.log(res)
						user.setUser('temp_th', res.data.temp_th);
						user.setUser('humi_th', res.data.humi_th);
						user.setUser('lsens_th', res.data.lsens_th);
						user.setUser('ppm_th', res.data.ppm_th);
						this.temp_th = user.getUser('temp_th');
						this.humi_th = user.getUser('humi_th');
						this.lsens_th = user.getUser('lsens_th');
						this.ppm_th = user.getUser('ppm_th');
						user.getAll();
					} else {
						console.error('Failed to get thresholds:', res.data.error);
					}
				},
				fail: (err) => {
					console.error('Request failed:', err);
				}
			});

			this.postThresholds();
		},
		onNavigationBarButtonTap: function(e) {
			uni.reLaunch({
				url: '/pages/login/login'
			})
		},
		methods: {
			postThresholds() {
				//发送到设备
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: '6j085UsKhq',
						device_name: 'd1',
						params: {
							"temp_th": this.temp_th,
							"humi_th": this.humi_th,
							"lsens_th": this.lsens_th,
							"ppm_th": this.ppm_th
						}
					},
					header: {
						'authorization': this.token // 自定义请求头信息
					},
					// 请求成功，打印提示信息，主要用于调试，可自定义
					success: (data) => {
						console.log('设备端阈值调整成功:');
						console.log(data)
					},
					fail: () => {
						console.log('设备端阈值调整失败');
					}
				});
			},
			// 滑动条变化事件的方法
			sliderChange(e, id) {
				console.log(id)
				console.log('value 发生变化：' + e.detail.value) // 控制台打印信息，调试用
				let key_th = {}
				if (id == 'slider1') {
					this.temp_th = e.detail.value
					// 更新温度阈值
					this.key_th = {
						temp_th: this.temp_th,
					};
				} else if (id == 'slider2') {
					this.humi_th = e.detail.value
					// 更新湿度阈值
					this.key_th = {
						humi_th: this.humi_th,
					};
				} else if (id == 'slider3') {
					this.lsens_th = e.detail.value
					// 更新湿度阈值
					this.key_th = {
						lsens_th: this.lsens_th,
					};
				} else if (id == 'slider4') {
					this.ppm_th = e.detail.value
					// 更新湿度阈值
					this.key_th = {
						ppm_th: this.ppm_th,
					};
				}
				console.log('value 发生变化：>>>>>' + stringify(this.key_th) + '<<<<<<<')
				//修改数据库
				uni.request({
					//url: "http://localhost:3000/update_thresholds",
					url: `http://${ip}/update_thresholds`, 
					method: 'PUT',
					data: {
						username: user.getUser('username'), // 替换为实际的用户名
						temp_th: this.temp_th,
						humi_th: this.humi_th,
						lsens_th: this.lsens_th,
						ppm_th: this.ppm_th
					},
					success: res => {
						if (res.statusCode === 200) {
							// 假设后端返回的是阈值对象
							console.log('数据库阈值修改成功返回')
							console.log(res);
							//console.log(this.ppm_th)
						} else {
							console.error('Failed to update thresholds:', res.data.error);
						}
					},
					fail: (err) => {
						console.error('Request failed:', err);
					}
				});
				
				//发送到设备端
				this.postThresholds();
				fetchData();
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