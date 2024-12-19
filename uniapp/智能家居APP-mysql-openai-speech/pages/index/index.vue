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
				<view class="weather-wind">风速: {{ weather.windspeed !== undefined ? weather.windspeed : '加载中...' }} km/h
				</view>
				<view class="weather-wind">空气{{ air.category !== undefined ? air.category : '加载中...' }} {{ air.aqi !==
					undefined ? air.aqi : '加载中...' }}</view>
				<view class="weather-wind">{{ suggest !== undefined ? suggest : '加载中...' }}</view>
				<view class="weather-wind">预警信息: {{ warning !== undefined ? warning : '加载中...' }}</view>
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

		<!-- 添加智能对话区域 -->
		<view class="chat-area">AI智能助手
			<scroll-view scroll-y class="chat-messages" :scroll-into-view="'msg-' + (messages.length - 1)">
				<view v-for="(msg, index) in messages" :key="index" :id="'msg-' + index"
					:class="['message', msg.type === 'user' ? 'user-message' : 'ai-message']">
					{{ msg.content }}
				</view>
			</scroll-view>

			<view class="input-area">
				<input type="text" v-model="userInput" placeholder="请输入您的问题或指令" @confirm="sendMessage" />

				<button class="voice-btn" @touchstart="startRecord" @touchend="stopRecord">
					<image src="../../static/voice.png" class="icon-voice" mode="aspectFit"></image>
				</button>

				<button class="send-btn" @tap="sendMessage">
					<image src="../../static/enter.png" class="icon-voice" mode="aspectFit"></image>
				</button>
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
	} = require('@/key.js')
	import {
		parse,
		stringify
	} from 'querystring'
	import {
		onLedSwitch,
		onBeepSwitch,
		setBrightness,
		setBeepVolume,
		setFanSpeed,
		setColorMode
	} from '@/pages/control/control.vue'
	import {
		setThreshold,
		temp_th,
		humi_th,
		lsens_th
	} from '../configPage/configPage.vue'

	export default {
		data() {
			return {
				token: '',
				temp: '0',
				humi: '0',
				lsens: '0',
				ppm: '0',

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
				condition4: false,
				userInput: '',
				chatResponse: '',
				messages: [],
				recording: false,
				recorderManager: null,
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
			const token = createCommonToken(params);
			this.token = token;
			// 存储token以供其他组件使用
			uni.setStorageSync('token', token);
			this.fetchDevData();
			this.getWeather();
			this.showPopup();
		},
		onShow() {
			this.fetchDevData();
			// this.showPopup();
			setInterval(() => { //定时获取
				this.fetchDevData();
			}, 6000);
			setInterval(() => {
				this.showPopup();
			}, 5000);
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
			},
			async sendMessage() {
				if (!this.userInput.trim()) return;

				this.messages.push({
					type: 'user',
					content: this.userInput
				});

				const userQuery = this.userInput;
				this.userInput = '';

				try {
					const response = await uni.request({
						url: 'https://open.api.gu28.top/v1/chat/completions',
						method: 'POST',
						data: {
							model: "gpt-4o-mini",
							messages: [{
								role: "system",
								content: "你是一个智能家居助手，负责查询传感器数据和当地天气情况、控制设备（包括led灯、蜂鸣报警器、温度阈值调整、湿度阈值调整、光强度阈值调整、烟雾浓度阈值调整、风扇挡位调整、彩灯模式调整。"
							}, {
								role: "user",
								content: userQuery
							}],
							functions: [{
									name: "controlLED",
									description: "控制LED灯的开关",
									parameters: {
										type: "object",
										properties: {
											status: {
												type: "boolean",
												description: "true表示开灯，false表示关灯"
											}
										},
										required: ["status"]
									}
								},
								{
									name: "controlBeep",
									description: "控制报警器的开关",
									parameters: {
										type: "object",
										properties: {
											status: {
												type: "boolean",
												description: "true表示开启报警器，false表示关闭报警器"
											}
										},
										required: ["status"]
									}
								},
								{
									name: "getSensorData",
									description: "获取传感器数据",
									parameters: {
										type: "object",
										properties: {
											sensor: {
												type: "string",
												enum: ["temperature", "humidity", "light", "smoke", "all",
													"weather"
												],
												description: "要查询的传感器类型"
											}
										},
										required: ["sensor"]
									}
								},
								{
									name: "setThreshold",
									description: "设置各种传感器的阈值",
									parameters: {
										type: "object",
										properties: {
											type: {
												type: "string",
												enum: ["temp", "humi", "lsens", "ppm"],
												description: "阈值类型：temp(温度)、humi(湿度)、lsens(光照)、ppm(烟雾)"
											},
											value: {
												type: "number",
												description: "阈值数值，范围0-100"
											}
										},
										required: ["type", "value"]
									}
								},
								{
									name: "setBrightness",
									description: "设置LED灯亮度",
									parameters: {
										type: "object",
										properties: {
											level: {
												type: "integer",
												enum: [0, 1, 2, 3],
												description: "亮度等级：0(暗)、1(较暗)、2(较亮)、3(亮)"
											}
										},
										required: ["level"]
									}
								},
								{
									name: "setBeepVolume",
									description: "设置报警器音量",
									parameters: {
										type: "object",
										properties: {
											level: {
												type: "integer",
												enum: [0, 1, 2, 3],
												description: "音量等级：0(小)、1(较小)、2(中)、3(大)"
											}
										},
										required: ["level"]
									}
								},
								{
									name: "setFanSpeed",
									description: "设置风扇速度",
									parameters: {
										type: "object",
										properties: {
											level: {
												type: "integer",
												enum: [0, 1, 2, 3],
												description: "速度等级：0(关)、1(慢)、2(中)、3(快)"
											}
										},
										required: ["level"]
									}
								},
								{
									name: "setColorMode",
									description: "设置彩灯/彩虹灯模式",
									parameters: {
										type: "object",
										properties: {
											mode: {
												type: "integer",
												enum: [0, 1, 2, 3],
												description: "彩灯模式：0(关)、1(模式一)、2(模式二)、3(模式三)"
											}
										},
										required: ["mode"]
									}
								}
							],
							function_call: "auto"
						},
						header: {
							'Authorization': 'Bearer sk-CBbXYAj2A480YQerqALXWgaecu4cCyPSaMrLBATZHH1QJxYM',
							'Content-Type': 'application/json'
						}
					});

					if (response.statusCode === 200) {
						const result = response.data.choices[0].message;

						// 显示AI的回复
						if (result.content) {
							this.messages.push({
								type: 'ai',
								content: result.content
							});
						}

						// 处理函数调用
						if (result.function_call) {
							const functionName = result.function_call.name;
							const args = JSON.parse(result.function_call.arguments);

							let functionResult = '';

							switch (functionName) {
								case 'controlLED':
									onLedSwitch({
										detail: {
											value: args.status
										}
									});
									functionResult = `已${args.status ? '打开' : '关闭'}LED灯`;
									break;

								case 'controlBeep':
									onBeepSwitch({
										detail: {
											value: args.status
										}
									});
									functionResult = `已${args.status ? '打开' : '关闭'}报警器`;
									break;

								case 'getSensorData':
									switch (args.sensor) {
										case 'temperature':
											functionResult = `当前温度是 ${this.temp}℃`;
											break;
										case 'humidity':
											functionResult = `当前湿度是 ${this.humi}%`;
											break;
										case 'light':
											functionResult = `当前光照强度是 ${this.lsens}lx`;
											break;
										case 'smoke':
											functionResult = `当前烟雾浓度是 ${this.ppm}ppm`;
											break;
										case 'weather':
											functionResult =
												`威海现在天气${this.weather.text}，温度${this.weather.temp}℃，湿度${this.weather.humi}%，空气质量${this.air.category}`;
											break;
										case 'all':
											functionResult =
												`当前数据如下：\n温度：${this.temp}℃\n湿度：${this.humi}%\n光照：${this.lsens}lx\n烟雾：${this.ppm}ppm\nLED灯：${this.led ? '开启' : '关闭'}\n报警器：${this.beep ? '开启' : '关闭'}`;
											break;
									}
									break;

								case 'setThreshold':
									if (args.value >= 0 && args.value <= 100) {
										setThreshold(args.type, args.value);
										const thresholdTypes = {
											temp: '温度',
											humi: '湿度',
											lsens: '光照',
											ppm: '烟雾'
										};
										functionResult = `已将${thresholdTypes[args.type]}阈值设置为${args.value}`;

										// 更新本地数据
										this[`${args.type}_th`] = args.value;
									}
									break;

								case 'setBrightness':
									setBrightness(args.level);
									const brightnessLevels = ['暗', '较暗', '较亮', '亮'];
									functionResult = `已将LED亮度设置为${brightnessLevels[args.level]}`;
									break;

								case 'setBeepVolume':
									setBeepVolume(args.level);
									const volumeLevels = ['小', '较小', '中', '大'];
									functionResult = `已将报警器音量设置为${volumeLevels[args.level]}`;
									break;

								case 'setFanSpeed':
									setFanSpeed(args.level);
									const speedLevels = ['关闭', '慢速', '中速', '快速'];
									functionResult = `已将风扇速度设置为${speedLevels[args.level]}`;
									break;

								case 'setColorMode':
									setColorMode(args.mode);
									const colorModes = ['关闭', '模式一', '模式二', '模式三'];
									functionResult = `已将彩灯设置为${colorModes[args.mode]}`;
									break;
							}

							if (functionResult) {
								this.messages.push({
									type: 'ai',
									content: functionResult
								});
							}
						}
					}
				} catch (error) {
					console.error('Error:', error);
					this.messages.push({
						type: 'ai',
						content: '抱歉，我遇到了一些问题，请稍后再试。'
					});
				}
			},
			// 修改语音识别相关方法
			startRecord() {
				// 请求录音权限
				// uni.authorize({
				// 	scope: 'scope.record',
				// 	success: () => {

				// 	},
				// 	fail: () => {
				// 		uni.showToast({
				// 			title: '请授权录音权限',
				// 			icon: 'none'
				// 		});
				// 	}
				// });

				this.recording = true;
				this.recorderManager = uni.getRecorderManager();

				// 配置录音参数
				const options = {
					duration: 60000, // 最长录音时间，单位ms
					sampleRate: 16000, // 采样率
					numberOfChannels: 1, // 录音通道数
					encodeBitRate: 96000, // 编码码率
					format: 'mp3', // 音频格式
					frameSize: 50 // 指定帧大小
				};

				this.recorderManager.onStart(() => {
					console.log('录音开始');
					uni.showToast({
						title: '开始录音',
						icon: 'none'
					});
				});

				this.recorderManager.onStop((res) => {
					this.recording = false;
					console.log('录音结束', res);
					uni.showToast({
						title: '录音结束',
						icon: 'none'
					});
					// 发送录音文件到语音识别API
					this.speechToText(res.tempFilePath);
				});

				this.recorderManager.start(options);
			},

			stopRecord() {
				if (this.recorderManager) {
					this.recorderManager.stop();
				}
			},

			async speechToText(audioFile) {
				try {
					// 显示加载提示
					uni.showLoading({
						title: '正在识别...'
					});

					const response = await uni.uploadFile({
						url: 'https://open.api.gu28.top/v1/audio/transcriptions',
						filePath: audioFile,
						name: 'file',
						formData: {
							model: 'whisper-1',
							language: 'zh'
						},
						header: {
							'Authorization': 'Bearer sk-CBbXYAj2A480YQerqALXWgaecu4cCyPSaMrLBATZHH1QJxYM',
							'Content-Type': 'multipart/form-data'
						}
					});

					// 隐藏加载提示
					uni.hideLoading();

					if (response.statusCode === 200) {
						const result = JSON.parse(response.data);
						if (result.text) {
							this.userInput = result.text.trim();
							this.sendMessage();
						} else {
							throw new Error('No transcription result');
						}
					} else {
						throw new Error('Speech recognition failed');
					}

				} catch (error) {
					console.error('Speech recognition error:', error);
					uni.hideLoading();
					uni.showToast({
						title: '语音识别失败，请重试',
						icon: 'none'
					});
					this.messages.push({
						type: 'ai',
						content: '语音识别失败，请重试或使用文字输入。'
					});
				}
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

	.chat-area {
		margin-top: 30rpx;
		border-radius: 15rpx;
		background-color: #fff;
		box-shadow: 0 4rpx 20rpx rgba(0, 0, 0, 0.1);
		padding: 20rpx;
	}

	.chat-messages {
		height: 400rpx;
		padding: 20rpx;
	}

	.message {
		margin: 20rpx 0;
		padding: 20rpx;
		border-radius: 10rpx;
		max-width: 80%;
	}

	.user-message {
		background-color: #13227a;
		color: #fff;
		margin-left: 70rpx;
	}

	.ai-message {
		background-color: #f0f0f0;
		color: #333;
	}

	.input-area {
		display: flex;
		padding: 20rpx;
		border-top: 5rpx solid #eee;
	}

	.input-area input {
		flex: 1;
		height: 80rpx;
		border: 1rpx solid #ddd;
		border-radius: 35rpx;
		padding: 0 30rpx;
		margin-right: 20rpx;
	}

	.voice-btn,
	.send-btn {
		width: 70rpx;
		height: 70rpx;
		border-radius: 35rpx;
		display: flex;
		align-items: center;
		justify-content: center;
		background-color: #13227a;
		color: #fff;
		margin: 10rpx;
	}
</style>