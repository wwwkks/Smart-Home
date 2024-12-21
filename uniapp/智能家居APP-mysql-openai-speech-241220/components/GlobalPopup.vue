<template>
	<view> <u-popup :show="show" mode="center" @close="close" @open="open">
			<view class="popup-content">
				<text class="popup-text">警告：温度超出阈值!!!</text>
				<button @click="close">点击关闭</button>
			</view>
		</u-popup>
		<u-popup :show="show2" mode="center" @close="close2" @open="open2">
			<view class="popup-content">
				<text class="popup-text">警告：湿度超出阈值!!!</text>
				<button @click="close2">点击关闭</button>
			</view>
		</u-popup>

		<u-popup :show="show3" mode="center" @close="close3" @open="open3">
			<view class="popup-content">
				<text class="popup-text">警告：光照强度超出阈值!!!</text>
				<button @click="close3">点击关闭</button>
			</view>
		</u-popup>

		<u-popup :show="show4" mode="center" @close="close4" @open="open4">
			<view class="popup-content">
				<text class="popup-text">警告：烟雾报警!!!</text>
				<button @click="close4">点击关闭</button>
			</view>
		</u-popup>
	</view>

</template>

<script>
	export default {
		name: 'GlobalPopup',
		data() {
			return {
				show: false,
				show2: false,
				show3: false,
				show4: false
			}
		},
		methods: {
			open() {
				// console.log('open');
			},
			close() {
				this.show = false
				// console.log('close');
			},
			showPopup() {
				this.show = true
			},

			open2() {
				// console.log('open');
			},
			close2() {
				this.show2 = false
				// console.log('close');
			},
			showPopup2() {
				this.show2 = true
			},

			open3() {
				// console.log('open');
			},
			close3() {
				this.show3 = false
				// console.log('close');
			},
			showPopup3() {
				this.show3 = true
			},

			open4() {
				// console.log('open');
			},
			close4() {
				this.show4 = false
				// console.log('close');
			},
			showPopup4() {
				this.show4 = true
			}

		},
	}
</script>

<style>
	.popup-content {
		padding: 40rpx;
		background-color: #fff;
		text-align: center;
	}

	button {
		margin-top: 20rpx;
		padding: 5rpx 15rpx;
		background-color: blue;
		color: #fff;
		border: none;
		border-radius: 5rpx;
	}

	.popup-text {
		color: #ff4d4f;
		/* 设置字体颜色为红色 */
		font-size: 32rpx;
		/* 设置字体大小 */
		font-weight: bold;
		/* 加粗字体 */
		margin-bottom: 20rpx;
		/* 与按钮之间的间距 */
	}
</style>