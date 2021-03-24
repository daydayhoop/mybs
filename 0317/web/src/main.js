import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import axios from 'axios'
import VueAxios from 'vue-axios'
import Vant from 'vant';
import 'vant/lib/index.css';

Vue.config.productionTip = false
Vue.use(VueAxios, axios)
Vue.use(Vant);

new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
