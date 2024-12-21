
var isReady=false;var onReadyCallbacks=[];
var isServiceReady=false;var onServiceReadyCallbacks=[];
var __uniConfig = {"pages":["pages/login/login","pages/register/register","pages/index/index","pages/control/control","pages/configPage/configPage"],"window":{"navigationBarTextStyle":"black","navigationBarTitleText":"智能家居控制台","navigationBarBackgroundColor":"#F8F8F8","backgroundColor":"#F8F8F8"},"tabBar":{"selectedColor":"#6d6d6d","color":"#ccc","list":[{"text":"首页","pagePath":"pages/index/index","iconPath":"../../static/home2.png","selectedIconPath":"../../static/home.png"},{"text":"控制","pagePath":"pages/control/control","iconPath":"../../static/config2.png","selectedIconPath":"../../static/config1.png"},{"text":"阈值","pagePath":"pages/configPage/configPage","iconPath":"../../static/threshold1.png","selectedIconPath":"../../static/threshold.png"}]},"darkmode":false,"nvueCompiler":"uni-app","nvueStyleCompiler":"uni-app","renderer":"auto","splashscreen":{"alwaysShowBeforeRender":true,"autoclose":false},"appname":"fan的智能家居","compilerVersion":"4.29","entryPagePath":"pages/login/login","networkTimeout":{"request":60000,"connectSocket":60000,"uploadFile":60000,"downloadFile":60000}};
var __uniRoutes = [{"path":"/pages/login/login","meta":{"isQuit":true},"window":{"navigationBarTitleText":"登录"}},{"path":"/pages/register/register","meta":{},"window":{"navigationBarTitleText":"注册"}},{"path":"/pages/index/index","meta":{"isQuit":true,"isTabBar":true},"window":{"navigationBarTitleText":"智能家居","titleNView":{"buttons":[{"text":"退出","type":"none","color":"#007AFF","background":"#000000","fontSize":"16px","float":"left"}]}}},{"path":"/pages/control/control","meta":{"isQuit":true,"isTabBar":true},"window":{"navigationBarTitleText":"控制台","titleNView":{"buttons":[{"text":"退出","type":"none","color":"#007AFF","background":"#000000","fontSize":"16px","float":"left"}]}}},{"path":"/pages/configPage/configPage","meta":{"isQuit":true,"isTabBar":true},"window":{"navigationBarTitleText":"阈值调整","titleNView":{"buttons":[{"text":"退出","type":"none","color":"#007AFF","background":"#000000","fontSize":"16px","float":"left"}]}}}];
__uniConfig.onReady=function(callback){if(__uniConfig.ready){callback()}else{onReadyCallbacks.push(callback)}};Object.defineProperty(__uniConfig,"ready",{get:function(){return isReady},set:function(val){isReady=val;if(!isReady){return}const callbacks=onReadyCallbacks.slice(0);onReadyCallbacks.length=0;callbacks.forEach(function(callback){callback()})}});
__uniConfig.onServiceReady=function(callback){if(__uniConfig.serviceReady){callback()}else{onServiceReadyCallbacks.push(callback)}};Object.defineProperty(__uniConfig,"serviceReady",{get:function(){return isServiceReady},set:function(val){isServiceReady=val;if(!isServiceReady){return}const callbacks=onServiceReadyCallbacks.slice(0);onServiceReadyCallbacks.length=0;callbacks.forEach(function(callback){callback()})}});
service.register("uni-app-config",{create(a,b,c){if(!__uniConfig.viewport){var d=b.weex.config.env.scale,e=b.weex.config.env.deviceWidth,f=Math.ceil(e/d);Object.assign(__uniConfig,{viewport:f,defaultFontSize:Math.round(f/20)})}return{instance:{__uniConfig:__uniConfig,__uniRoutes:__uniRoutes,global:void 0,window:void 0,document:void 0,frames:void 0,self:void 0,location:void 0,navigator:void 0,localStorage:void 0,history:void 0,Caches:void 0,screen:void 0,alert:void 0,confirm:void 0,prompt:void 0,fetch:void 0,XMLHttpRequest:void 0,WebSocket:void 0,webkit:void 0,print:void 0}}}});
