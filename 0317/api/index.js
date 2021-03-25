const express = require('express');
//引入框架模块
const app = express();
//创建服务器--创建一个应用
const port = 3000;
//声明端口
const fetch = require('node-fetch');
//加载fetch接口获取后端数据
const cors = require('cors');
app.use(cors());
//使用cros解决跨域问题
app.options('*', cors());
//该方法用于请求服务器告知其支持哪些其他的功能和方法。
//app.use(express.json())
const device_id = "691338057"
const api_key = "uTGrygpSqAZcK5LXjxkgVlsxCmc="
app.get('/', (req, res) => {
    res.send('Hello World, from express');
});
//异步获取数据传回getData目录中
app.get('/getData', async (req, res) => {
   const response1 = await fetch(encodeURI(`http://api.heclouds.com/devices/${device_id}/datastreams/温度`), {

        headers: {
            'api-key': api_key
        },

    });


    const response2 = await fetch(encodeURI(`http://api.heclouds.com/devices/${device_id}/datastreams/湿度`), {

        headers: {
            'api-key': api_key
        },

    });

    const response3 = await fetch(encodeURI(`http://api.heclouds.com/devices/${device_id}/datastreams/烟雾`), {

        headers: {
            'api-key': api_key
        },

    });

    const response4 = await fetch(encodeURI(`http://api.heclouds.com/devices/${device_id}/datastreams/是否有人`), {

        headers: {
            'api-key': api_key
        },

    });
    //fetch()接收到的response是一个 Stream 对象，response.json()是一个异步操作，取出所有内容，并将其转为 JSON 对象。
    var data1 = await response1.json()
    var data2 = await response2.json()
    var data3 = await response3.json()
    var data4 = await response4.json()
    //根据onenet返回的数据提取实时数据
    const data = {
        temperature: data1.data.current_value,
        humidity: data2.data.current_value,
        mq2: data3.data.current_value,
        pir: data4.data.current_value
    }

    res.json(data);

});
//处理客户端get请求中的参数
app.get('/cmd/:t', async (req, res) => {
    const { t } = req.params
    console.info(t);
    //将得到的参数放入body中通过post方式将指令传递给onenet平台
    const response1 = await fetch(encodeURI(`http://api.heclouds.com/cmds?device_id=${device_id}`), {
        method: 'POST',
        headers: {
            'api-key': api_key
        },
        body: t
    });

    var data1 = await response1.json()

    res.json(data1);
    //打印onenet平台的返回值{
    //返回示例1：成功创建命令
    //     "errno": 0,
    //     "error": "succ",
    //     "data": {
    //         "cmd_uuid": "81572aae-fc34-5deb-8f06-ab45d73cb12b"
    //     }
    // }
    // 返回示例2：设备不在线
    //
    // {
    //     "errno": 10,
    //     "error": "device not online: 8029377"
    // }
});

//监听端口
app.listen(port, async () => {
    console.log(`正在使用${port}!`)
})


