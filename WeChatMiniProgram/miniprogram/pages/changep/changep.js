// pages/changep/changep.js
var app = getApp();
let oldPass = null;
let newPass = null;
let newPass2 = null;
const db = wx.cloud.database()
const users = db.collection('user')
Page({

  /**
   * 页面的初始数据
   */
  data: {
    iosDialog1:false

  },

  inputOld(event){
    oldPass = event.detail.value
  },

  inputNew(event) {
    newPass = event.detail.value
  },

  inputNew2(event) {
    newPass2 = event.detail.value
  },


  changeConf: function(){
    this.setData({
      iosDialog1:true
    })
  },

  close(){
    this.setData({
      iosDialog1: false
    });  
  },

  confirm(){
    console.log("data1?", app.globalData.userpassword)
    console.log("data2?", app.globalData.userid)
    if (oldPass == app.globalData.userpassword && newPass == newPass2){
      console.log("YES!")
      users.doc(app.globalData.userid).update({
        data:{
          password:newPass
        }
      })
      wx.showToast({
        title: '修改成功',
        icon:"success",
        duration:2500
      })
      wx.navigateBack({
        delta: 1
      })
  }else{
      wx.showToast({
        title: '密码错误，修改失败',
        icon:"none"
      })
  }
    this.setData({
      iosDialog1: false
    });  

    
  },


  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

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

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})