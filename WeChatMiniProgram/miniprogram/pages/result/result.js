// pages/result/result.js
var week
var day
var time
var location
var mydate= new Date()
const db=wx.cloud.database()
const cart_col=db.collection('cart')
var app = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    // dialogShow: false,
    // buttons: [{ text: '取消' }, { text: '确定' }],
    datalist:[],
    type:0
  },

  openConfirm: async function (e) {
    // this.setData({
    //   dialogShow: true
    // })    
    let { item } = e.currentTarget.dataset
    console.log("success", item._id)
    // const type=app.globalData.usertype
    // console.log("check",type)
    if (app.globalData.usertype==1){
    try{
      let res= await cart_col.doc(item._id).get()
      console.log('有值')
      wx.showToast({
        title: '该课室已被预约',
        image:'/images/round_close.png',
        duration:2500
      })
    }catch(err){
      console.log('无值')
      await cart_col.add({
        data:{
          _id:item._id,
          week:item.week,
          day:item.day,
          time:item.time,
          location:item.location,
          room:item.room,
          useraccount: app.globalData.useraccount,
          username: app.globalData.username,
          applydate:mydate.toLocaleString(),
          check:'0'
        }
      })
      wx.showToast({
        title: '预约成功',
        image:'/images/round_check.png',
        duration:2500
      })
    }
  }else{
      wx.showToast({
        title: '无权限预约',
        image: '/images/round_close.png',
        duration: 2500
      })
  }

  },

  // tapDialogButton(e) {
  //   console.log("which is",e.detail.index)//index==1为确定，0为取消
  //   const t=this.openConfirm();
  //   console.log("deliver ok",t)
  //   this.setData({
  //     dialogShow: false
  //   })
  // },
  

handleTabclick(event){
  // console.log('success',event)
  const index=event.detail.index
  this.setData({
    type:index
  })
},






  onLoad: function (options) {
    week=options.week;
    day=options.day;
    time=options.time;
    location=options.location;
    if (week == 0 && day == 0 && time == 0 && location == 2) {
      let that = this
      wx.cloud.database().collection("0002").get({
        success(res) {
          console.log('成功', res.data)
          that.setData({
            datalist: res.data
          })
        }, fail(res) {
          console.log('失败', res)
        }
      })
    }
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

// apihuoqu(){
//   let that = this
// wx.cloud.database().collection("").get({
//   success(res){
//     console.log('成功',res.data)
//       that.setData({
//       datalist:res.data
//       })
//   },fail(res){
//     console.log('失败',res)
//   }
// })
// },

// getdata(){
//   let that = this
//   wx.cloud.callFunction({
//     name:"getshuju",
//     success(res){
//       console.log('请求成功',res.result.data.length)//返回数据的结构
//       let length = res.result.data.length
//     },fail(res){
//       console.log('请求失败',res)
//     }
//   })
//   console.log(length)
// },
  
})