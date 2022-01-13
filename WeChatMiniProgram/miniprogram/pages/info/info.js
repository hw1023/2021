// pages/info/info.js
var app = getApp()

Page({

  /**
   * 页面的初始数据
   */
  data: {
    userA:null,
    userN:null,
    userT:null,
    userTa:null
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.setData({
      userN: app.globalData.username,
      userA: app.globalData.useraccount
    })
    if (app.globalData.usertype==0){
      this.setData({
        userT:"学生",
        userTa:"无"
      })
    }else{
      this.setData({
        userT: "教师",
        userTa: "有"
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

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})