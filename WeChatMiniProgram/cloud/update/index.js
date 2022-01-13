// 云函数入口文件
const cloud = require('wx-server-sdk')

cloud.init()

// 云函数入口函数
exports.main = async (event, context) => {
  i=event.id
  p=event.password
  try {
    return await db.collection('user').doc(i).update({
      // data 传入需要局部更新的数据
      data: {
        // 表示将 done 字段置为 true
        password: p
      }
    })
  } catch (e) {
    console.error(e)
  }
}