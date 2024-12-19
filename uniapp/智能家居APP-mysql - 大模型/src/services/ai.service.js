const { OpenAI } = require('openai');

// 初始化OpenAI配置
const openai = new OpenAI({ 
  apiKey: process.env.OPENAI_API_KEY,
  baseURL: process.env.OPENAI_BASE_URL
});

// 分析命令的辅助函数
function analyzeCommand(text) {
  // LED控制指令
  const ledMatch = text.match(/开灯|关灯|调整亮度到(\d+)/);
  if (ledMatch) {
    if (ledMatch[1]) {
      return {
        device: 'bright',
        status: parseInt(ledMatch[1])
      };
    }
    return {
      device: 'led',
      status: ledMatch[0] === '开灯'
    };
  }

  // 蜂鸣器控制指令
  const beepMatch = text.match(/开启报警器|关闭报警器|调整音量到(\d+)/);
  if (beepMatch) {
    if (beepMatch[1]) {
      return {
        device: 'sound',
        status: parseInt(beepMatch[1])
      };
    }
    return {
      device: 'beep',
      status: beepMatch[0] === '开启报警器'
    };
  }

  // 风扇控制指令
  const fanMatch = text.match(/开启风扇|关闭风扇|调整风速到(\d+)/);
  if (fanMatch) {
    if (fanMatch[1]) {
      return {
        device: 'speed',
        status: parseInt(fanMatch[1])
      };
    }
    return {
      device: 'speed',
      status: fanMatch[0] === '开启风扇' ? 1 : 0
    };
  }

  return null;
}

// 分析环境数据
function analyzeEnvironment(deviceData) {
  let warning = null;
  if (deviceData.temp > 30) {
    warning = "温度过高,建议开启风扇降温";
  } else if (deviceData.humi > 80) {
    warning = "湿度过高,建议通风";
  } else if (deviceData.ppm > 80) {
    warning = "检测到烟雾浓度异常,建议检查";
  }
  return warning;
}

// 生成系统提示信息
function generateSystemPrompt(deviceData) {
  return `你是一个智能家居助手,可以帮助用户控制设备和查看环境数据。当前设备状态:
温度: ${deviceData.temp}℃
湿度: ${deviceData.humi}%
光照: ${deviceData.lsens}lx
烟雾: ${deviceData.ppm}ppm
LED灯: ${deviceData.led ? '开' : '关'}
LED亮度: ${deviceData.bright}
蜂鸣器: ${deviceData.beep ? '开' : '关'}
蜂鸣器音量: ${deviceData.sound}
风扇速度: ${deviceData.speed}

你可以:
1. 回答关于环境数据的问题
2. 控制设备开关
3. 调整设备参数
4. 根据环境数据给出建议
5. 发出警告提醒

请用自然、友好的语气回复。`;
}

// 处理聊天请求
export async function handleAIChat(message, deviceData) {
  try {
    const completion = await openai.chat.completions.create({
      model: "gpt-3.5-turbo",
      messages: [
        {
          role: "system",
          content: generateSystemPrompt(deviceData)
        },
        {
          role: "user",
          content: message
        }
      ],
      temperature: 0.7,
      max_tokens: 1000
    });

    const reply = completion.choices[0].message.content;
    const command = analyzeCommand(reply);
    const warning = analyzeEnvironment(deviceData);
    
    return {
      message: reply,
      command: command,
      warning: warning
    };
    
  } catch (error) {
    console.error('OpenAI API错误:', error);
    throw new Error('AI助手暂时无法回应,请稍后再试。');
  }
} 