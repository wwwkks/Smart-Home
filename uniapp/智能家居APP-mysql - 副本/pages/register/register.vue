<template>
  <view class="container">
    <view class="register-form">
      <input type="text" v-model="registerForm.username" placeholder="请输入用户名" />
      <input type="password" v-model="registerForm.password" placeholder="请输入密码" />
      <input type="password" v-model="registerForm.confirmPassword" placeholder="请再次输入密码" />
      <button @click="handleRegister">注册</button>
      <navigator url="/pages/login/login">已有账号？去登录</navigator>
    </view>
  </view>
</template>

<script>
export default {
  data() {
    return {
      registerForm: {
        username: '',
        password: '',
        confirmPassword: ''
      }
    };
  },
  methods: {
    handleRegister() {
      // 这里需要发送请求到后端进行注册
      if (this.registerForm.password !== this.registerForm.confirmPassword) {
        uni.showToast({
          title: '两次输入的密码不一致',
          icon: 'none'
        });
        return;
      }
      uni.request({
        url: 'http://localhost:3000/register', // 注意：这里的url需要根据实际情况调整
        method: 'POST',
        data: {
          username: this.registerForm.username,
          password: this.registerForm.password
        },
        success: (res) => {
          if (res.statusCode === 201) {
            uni.showToast({
              title: '注册成功'
            });

            // 注册成功后的操作，比如跳转到登录页面
            uni.reLaunch({
              url: "/pages/login/login"
            })
          } else {
            uni.showToast({
              title: '注册失败',
              icon: 'none'
            });
          }
        },
        fail: (err) => {
          console.error(err);
          uni.showToast({
            title: '请求失败',
            icon: 'none'
          });
        }
      });
    }
  }
};
</script>

<style>
.container {
  display: flex;
  justify-content: center;
  align-items: center;
  height: 100vh;
  background: linear-gradient(135deg, #f5f7fa, #c3cfe2);
  /* 添加渐变背景 */
}

.register-form {
  display: flex;
  flex-direction: column;
  width: 80%;
  padding: 40px;
  /* 增加内边距 */
  background-color: #ffffff;
  border-radius: 12px;
  /* 增加圆角 */
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
  /* 增加阴影 */
}

.register-form input {
  height: 45px;
  /* 增加高度 */
  margin-bottom: 20px;
  padding: 0 15px;
  border: 1px solid #dcdfe6;
  border-radius: 4px;
  font-size: 14px;
  transition: border-color 0.3s;
  /* 添加过渡效果 */
}

.register-form input:focus {
  border-color: #409eff;
  outline: none;
  box-shadow: 0 0 5px rgba(64, 158, 255, 0.5);
  /* 添加聚焦阴影 */
}

.register-form button {
  height: 45px;
  /* 增加高度 */
  background-color: #409eff;
  color: #ffffff;
  border: none;
  border-radius: 4px;
  font-size: 16px;
  cursor: pointer;
  transition: background-color 0.3s, transform 0.3s;
  /* 添加过渡效果 */
}

.register-form button:hover {
  background-color: #66b1ff;
  /* 悬停时改变背景色 */
  transform: translateY(-2px);
  /* 悬停时轻微上移 */
}

.register-form button:active {
  background-color: #3a8ee6;
  transform: translateY(0);
  /* 点击时恢复位置 */
}
</style>
