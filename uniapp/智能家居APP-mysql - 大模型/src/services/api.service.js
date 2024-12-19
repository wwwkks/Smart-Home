import { ip } from '@/common.js';
import { handleAIChat } from './ai.service';

// 处理聊天请求
export async function sendChatMessage(message, deviceData) {
  try {
    // 直接在前端处理AI对话
    const aiResponse = await handleAIChat(message, deviceData);
    return aiResponse;
  } catch (error) {
    console.error('AI对话失败:', error);
    throw new Error('AI对话失败,请稍后再试');
  }
}

// 处理语音上传
export async function uploadVoice(filePath) {
  try {
    // 使用本地语音识别API或第三方服务
    const recognition = new webkitSpeechRecognition();
    recognition.lang = 'zh-CN';
    recognition.continuous = false;
    recognition.interimResults = false;
    
    return new Promise((resolve, reject) => {
      recognition.onresult = (event) => {
        const text = event.results[0][0].transcript;
        resolve({ text });
      };
      
      recognition.onerror = (error) => {
        reject(new Error('语音识别失败'));
      };
      
      recognition.start();
    });
  } catch (error) {
    console.error('语音识别失败:', error);
    throw new Error('语音识别失败');
  }
} 