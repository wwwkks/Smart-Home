(global["webpackJsonp"]=global["webpackJsonp"]||[]).push([["pages/control/control"],{"39a2":function(e,t,o){"use strict";var n=o("cbcf"),c=o.n(n);c.a},"747d":function(e,t,o){"use strict";o.r(t);var n=o("d29f"),c=o("f596");for(var i in c)["default"].indexOf(i)<0&&function(e){o.d(t,e,(function(){return c[e]}))}(i);o("39a2");var a=o("828b"),s=Object(a["a"])(c["default"],n["b"],n["c"],!1,null,null,null,!1,n["a"],void 0);t["default"]=s.exports},a6ac:function(e,t,o){"use strict";(function(e,t){var n=o("47a9");o("f055");n(o("3240"));var c=n(o("747d"));e.__webpack_require_UNI_MP_PLUGIN__=o,t(c.default)}).call(this,o("3223")["default"],o("df3c")["createPage"])},c20a:function(e,t,o){"use strict";(function(e){Object.defineProperty(t,"__esModule",{value:!0}),t.setFanSpeed=t.setColorMode=t.setBrightness=t.setBeepVolume=t.onLedSwitch=t.onBeepSwitch=t.default=void 0;o("3240");var n=o("4543"),c=n.createCommonToken,i=c({author_key:"rQ9SmPK1mdPtwhRto/+8RNqy57QHHxqjd0GjEzLSkkiE4m2tBg3M43LRK+kvs7sW",version:"2022-05-01",user_id:"412104"}),a={data:function(){return{led:!1,beep:!1,list1:["暗","较暗","较亮","亮"],list2:["小","较小","中","大"],list3:["关","慢","中","快"],list4:["关","模式一","模式二","模式三"],curNow1:0,curNow2:0,curNow3:0,curNow4:0}},onLoad:function(){this.fetchData()},onShow:function(){var e=this;setInterval((function(){e.fetchData()}),4e3),setInterval((function(){e.updateState()}),15e3)},onNavigationBarButtonTap:function(t){e.reLaunch({url:"/pages/login/login"})},methods:{onLedSwitch:function(t){var o=t.detail.value;e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{led:o}},header:{authorization:i},success:function(){console.log("LED "+(o?"ON":"OFF")+" !")}})},onBeepSwitch:function(t){var o=t.detail.value;e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{beep:o}},header:{authorization:i},success:function(){console.log("BEEP "+(o?"ON":"OFF")+" !")}})},sectionChange:function(e){this.curNow=e},updateState:function(){e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{bright:this.curNow1,sound:this.curNow2,speed:this.curNow3,gramode:this.curNow4}},header:{authorization:i},success:function(){console.log("当前状态发送成功")},fail:function(){console.log("当前状态发送失败")}})},fetchData:function(){var t=this;e.request({url:"https://iot-api.heclouds.com/thingmodel/query-device-property",method:"GET",data:{product_id:"6j085UsKhq",device_name:"d1"},header:{authorization:i},success:function(e){console.log("获取设备数据"),console.log(e.data),t.beep="true"===e.data.data[0].value,t.led="true"===e.data.data[5].value,console.log("led:"+t.led),console.log("beep:"+t.beep)}})},handleClick:function(e){this.curNow1=e,this.updateState()},handleBeepClick:function(e){this.curNow2=e,this.updateState()},handleSpeedClick:function(e){this.curNow3=e,this.updateState()},handleModeClick:function(e){this.curNow4=e,this.updateState()}}};t.default=a;t.onLedSwitch=function(t){console.log("AI调试:灯");var o=t.detail.value;e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{led:o}},header:{authorization:i},success:function(){console.log("LED "+(o?"ON":"OFF")+" !")}})};t.onBeepSwitch=function(t){console.log("AI调试:beep");var o=t.detail.value;e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{beep:o}},header:{authorization:i},success:function(){console.log("BEEP "+(o?"ON":"OFF")+" !")}})};t.setBrightness=function(t){console.log("AI调试:LED亮度",t),e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{bright:t}},header:{authorization:i},success:function(){console.log("LED brightness set to level:",t)}})};t.setBeepVolume=function(t){console.log("AI调试:报警器音量",t),e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{sound:t}},header:{authorization:i},success:function(){console.log("Beep volume set to level:",t)}})};t.setFanSpeed=function(t){console.log("AI调试:风扇速度",t),e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{speed:t}},header:{authorization:i},success:function(){console.log("Fan speed set to level:",t)}})};t.setColorMode=function(t){console.log("AI调试:彩灯模式",t),e.request({url:"https://iot-api.heclouds.com/thingmodel/set-device-property",method:"POST",data:{product_id:"6j085UsKhq",device_name:"d1",params:{gramode:t}},header:{authorization:i},success:function(){console.log("Color light mode set to:",t)}})}}).call(this,o("df3c")["default"])},cbcf:function(e,t,o){},d29f:function(e,t,o){"use strict";o.d(t,"b",(function(){return c})),o.d(t,"c",(function(){return i})),o.d(t,"a",(function(){return n}));var n={uSubsection:function(){return Promise.all([o.e("common/vendor"),o.e("uni_modules/uview-ui/components/u-subsection/u-subsection")]).then(o.bind(null,"49d3"))}},c=function(){var e=this.$createElement;this._self._c},i=[]},f596:function(e,t,o){"use strict";o.r(t);var n=o("c20a"),c=o.n(n);for(var i in n)["default"].indexOf(i)<0&&function(e){o.d(t,e,(function(){return n[e]}))}(i);t["default"]=c.a}},[["a6ac","common/runtime","common/vendor"]]]);