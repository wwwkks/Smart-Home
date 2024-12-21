<!-- <template>
	<view>
		<GlobalPopup ref="globalPopup" />
		<view>
			<router-view></router-view>
		</view>
	</view>
</template> -->

<style lang="scss">
/* 注意要写在第一行，同时给style标签加入lang="scss"属性 */
@import "@/uni_modules/uview-ui/index.scss";
</style>


<script>
//import GlobalPopup from './components/GlobalPopup.vue'
export default {

	onLaunch: function () {
		console.log('App Launch');
		// 初始化用户信息
		this.$store.dispatch('initUserInfo')
	},
	onShow: function () {
		console.log('App Show')
	},
	onHide: function () {
		console.log('App Hide')
	},
	// components: {
	// 	GlobalPopup
	// }
}
</script>