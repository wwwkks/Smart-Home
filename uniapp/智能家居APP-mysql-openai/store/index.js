import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

const store = new Vuex.Store({
  state: {
    userInfo: {
      username: '',
      temp_th: 100,
      humi_th: 100,
      lsens_th: 100,
      ppm_th: 100
    }
  },
  mutations: {
    // 设置用户信息
    SET_USER_INFO(state, userInfo) {
      state.userInfo = {...state.userInfo, ...userInfo}
      // 同时存储到本地存储
      uni.setStorageSync('userInfo', state.userInfo)
    },
    // 清除用户信息
    CLEAR_USER_INFO(state) {
      state.userInfo = {
        username: '',
        temp_th: 100,
        humi_th: 100,
        lsens_th: 100,
        ppm_th: 100
      }
      uni.removeStorageSync('userInfo')
    }
  },
  actions: {
    // 初始化用户信息(从本地存储恢复)
    initUserInfo({ commit }) {
      const userInfo = uni.getStorageSync('userInfo')
      if (userInfo) {
        commit('SET_USER_INFO', userInfo)
      }
    }
  }
})

export default store