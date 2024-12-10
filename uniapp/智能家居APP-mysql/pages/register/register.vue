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
				url:"/pages/login/login"
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
}

.register-form {
  display: flex;
  flex-direction: column;
  width: 80%;
}
</style>
