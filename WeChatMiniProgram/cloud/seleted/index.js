// 云函数入口文件
const cloud = require('wx-server-sdk')

cloud.init()

// 云函数入口函数
exports.main = async (event, context) => {
  let duration=event.duration
  let week=event.week
  let time=event.time
  let type=event.type

  datalist:[]

  wx.cloud.callFunction({
    name:"getshuju",
    success(res) {
      console.log('请求成功', res.result.data[0].duration),//返回数据的结构
        that.setData({
          datalist: res.result.data
        })
    }, fail(res) {
      console.log('请求失败', res)
    }
  })

  return 
}