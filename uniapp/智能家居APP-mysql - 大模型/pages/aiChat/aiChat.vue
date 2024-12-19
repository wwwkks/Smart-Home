<template>
    <view class="chat-container">
        <!-- 聊天记录区域 -->
        <scroll-view class="chat-list" scroll-y scroll-into-view="msg-{{scrollToMessage}}">
            <view v-for="(item, index) in chatHistory" :key="index" :id="'msg-' + index" class="chat-item">
                <view :class="['message', item.role === 'user' ? 'user-message' : 'ai-message']">
                    <text>{{ item.content }}</text>
                </view>
            </view>
        </scroll-view>

        <!-- 输入区域 -->
        <view class="input-area">
            <view class="input-box">
                <input v-model="inputMessage" type="text" placeholder="请输入您的问题" @confirm="sendMessage" />
                <button class="voice-btn" @touchstart="startRecord" @touchend="stopRecord">
                    <text class="iconfont">🎤</text>
                </button>
            </view>
            <button class="send-btn" @tap="sendMessage">发送</button>
        </view>
    </view>
</template>

<script>
import { sendChatMessage, uploadVoice } from '@/services/api.service';
import { ip } from '@/common.js'

export default {
    data() {
        return {
            chatHistory: [],
            inputMessage: '',
            scrollToMessage: 0,
            isRecording: false,
            deviceData: null,
            token: '',
            warning: null
        }
    },

    onLoad() {
        const params = {
            author_key: 'your_key',
            version: '2022-05-01',
            user_id: '412104',
        }
        this.token = createCommonToken(params);

        this.fetchDeviceData();
        setInterval(() => {
            this.fetchDeviceData();
        }, 10000);
    },

    methods: {
        // 获取设备数据
        async fetchDeviceData() {
            try {
                const res = await uni.request({
                    url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
                    method: 'GET',
                    data: {
                        product_id: '6j085UsKhq',
                        device_name: 'd1'
                    },
                    header: {
                        'authorization': this.token
                    }
                });

                this.deviceData = {
                    temp: res.data.data[12].value,
                    humi: res.data.data[3].value,
                    lsens: res.data.data[6].value,
                    ppm: res.data.data[8].value,
                    led: res.data.data[4].value === 'true',
                    beep: res.data.data[0].value === 'true',
                    bright: res.data.data[1].value,
                    sound: res.data.data[2].value,
                    speed: res.data.data[5].value
                };

                return this.deviceData;
            } catch (error) {
                console.error('获取设备数据失败:', error);
                return null;
            }
        },

        // 控制设备
        async controlDevice(device, status) {
            try {
                await uni.request({
                    url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
                    method: 'POST',
                    data: {
                        product_id: '6j085UsKhq',
                        device_name: 'd1',
                        params: {
                            [device]: status
                        }
                    },
                    header: {
                        'authorization': this.token
                    }
                });
                return true;
            } catch (error) {
                console.error('控制设备失败:', error);
                return false;
            }
        },

        // 发送消息到服务器
        async sendMessage() {
            if (!this.inputMessage.trim()) return;

            this.chatHistory.push({
                role: 'user',
                content: this.inputMessage
            });

            const userMessage = this.inputMessage;
            this.inputMessage = '';

            await this.fetchDeviceData();

            try {
                const aiResponse = await sendChatMessage(userMessage, this.deviceData);

                if (aiResponse.warning) {
                    this.warning = aiResponse.warning;
                    uni.showToast({
                        title: aiResponse.warning,
                        icon: 'none',
                        duration: 3000
                    });
                }

                if (aiResponse.command) {
                    const { device, status } = aiResponse.command;
                    await this.controlDevice(device, status);

                    this.chatHistory.push({
                        role: 'system',
                        content: `已${status ? '开启' : '关闭'}${device === 'led' ? 'LED灯' : '报警器'}`
                    });
                }

                this.chatHistory.push({
                    role: 'assistant',
                    content: aiResponse.message
                });

                this.scrollToMessage = this.chatHistory.length - 1;

            } catch (error) {
                console.error('发送消息失败:', error);
                this.chatHistory.push({
                    role: 'assistant',
                    content: error.message
                });
            }
        },

        // 开始录音
        startRecord() {
            this.isRecording = true;
            // 调用录音API
            uni.startRecord({
                success: () => {
                    uni.showToast({
                        title: '开始录音',
                        icon: 'none'
                    });
                }
            });
        },

        // 结束录音并发送语音
        async stopRecord() {
            if (!this.isRecording) return;
            this.isRecording = false;

            uni.stopRecord({
                success: async (res) => {
                    try {
                        const { text } = await uploadVoice(res.tempFilePath);
                        this.inputMessage = text;
                        await this.sendMessage();

                    } catch (error) {
                        uni.showToast({
                            title: error.message,
                            icon: 'none'
                        });
                    }
                }
            });
        }
    }
}
</script>

<style>
.chat-container {
    display: flex;
    flex-direction: column;
    height: 100vh;
    background: #f5f5f5;
}

.chat-list {
    flex: 1;
    padding: 20rpx;
}

.chat-item {
    margin-bottom: 20rpx;
}

.message {
    max-width: 70%;
    padding: 20rpx;
    border-radius: 10rpx;
    word-break: break-all;
}

.user-message {
    margin-left: auto;
    background: #007AFF;
    color: #fff;
}

.ai-message {
    margin-right: auto;
    background: #fff;
}

.input-area {
    padding: 20rpx;
    background: #fff;
    display: flex;
    align-items: center;
}

.input-box {
    flex: 1;
    display: flex;
    align-items: center;
    background: #f5f5f5;
    border-radius: 10rpx;
    padding: 0 20rpx;
    margin-right: 20rpx;
}

.input-box input {
    flex: 1;
    height: 80rpx;
}

.voice-btn {
    padding: 0 20rpx;
    background: none;
}

.send-btn {
    width: 120rpx;
    height: 80rpx;
    line-height: 80rpx;
    text-align: center;
    background: #007AFF;
    color: #fff;
    border-radius: 10rpx;
}
</style>