<template>
  <div>
      <van-grid :border="false" :column-num="4"  gutter="10">
          <van-grid-item>
              <van-image  fit="contain" width="100" height="100"
                          :src="require('../assets/icon/1.jpg')" />
              <div>温度:{{oneData.temperature}}</div>
          </van-grid-item>
          <van-grid-item>
              <van-image fit="contain" width="100" height="100"
                         :src="require('../assets/icon/2.jpg')"/>
              <div>湿度：{{oneData.humidity}}</div>
          </van-grid-item>
          <van-grid-item id=1>
              <van-image fit="contain" width="100" height="100"
                         :src="require('../assets/icon/3.jpg')" />
              <div>烟雾：{{oneData.mq2}}</div>
          </van-grid-item>
          <van-grid-item>
              <van-image fit="contain" width="100" height="100"
                         :src="require('../assets/icon/4.jpg')" />
              <div>是否有人：{{oneData.pir == '0' ? '无人' : '有人'}}</div>
          </van-grid-item>
      </van-grid>
  <div class="mydiv">
    <div style="margin: 16px" >
      <van-button type="primary" @click="open">开灯</van-button>
    </div>
    <div style="margin: 16px" >
      <van-button type="info" @click="close">关灯</van-button>
    </div>
  </div>
	<van-cell-group>
	  <van-field v-model="value" label="阈值" placeholder="请输入最大值" />
	</van-cell-group>
	
	<van-button @click="ok" type="danger">设置阈值</van-button>



  </div>
</template>
<style>
    .mydiv {
        display:flex;
        justify-content:center;
    }
</style>
<script>
export default {
  data() {

      return {
		value:0,
      oneData: { temperature: 0, humidity: 0, mq2: 0, pir: "" },
    };

  },

  methods: {
	  ok(){

		  this.axios.get("http://localhost:3000/cmd/"+this.value).then((res) => {
		    
		    console.info(res.data);
		  });
	  },
    close() {
      this.axios.get("http://localhost:3000/cmd/close").then((res) => {
        
        console.info(res.data);
      });
    },
    open() {
      this.axios.get("http://localhost:3000/cmd/open").then((res) => {
      
        console.info(res.data);
      });
    },
  },
  mounted() {
    setInterval(() => {
      this.axios.get("http://localhost:3000/getData").then((res) => {
        this.oneData = res.data;
        console.info(res.data);
      });
    }, 1000);
  },



};
</script>
