const express = require('express');
const app = express();
const port = 3000;
const fetch = require('node-fetch');
const cors = require('cors');
app.use(cors());
app.options('*', cors());
app.use(express.json())
const device_id = "691338057"
const api_key = "uTGrygpSqAZcK5LXjxkgVlsxCmc="
app.get('/', (req, res) => {
    res.send('Hello World, from express');
});
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
    var data1 = await response1.json()
    var data2 = await response2.json()
    var data3 = await response3.json()
    var data4 = await response4.json()
    const data = {
        temperature: data1.data.current_value,
        humidity: data2.data.current_value,
        mq2: data3.data.current_value,
        pir: data4.data.current_value
    }

    res.json(data);

});

app.get('/cmd/:t', async (req, res) => {
    const { t } = req.params
    console.info(t);
    const response1 = await fetch(encodeURI(`http://api.heclouds.com/cmds?device_id=${device_id}`), {
        method: 'POST',
        headers: {
            'api-key': api_key
        },
        body: t
    });

    var data1 = await response1.json()

    res.json(data1);
});


app.listen(port, async () => {
    console.log(`正在使用${port}!`)
})


