// pages/mylist/mylist.js
var app = getApp()
const db = wx.cloud.database()
const cart_col = db.collection('cart')
Page({

  /**
   * 页面的初始数据
   */
  data: {
    uA:null,
    datalist:[]
  },
//取消预约
  delConfirm: function (e) {
    let { item } = e.currentTarget.dataset//得到传递的item数据
    console.log("success", item._id)
    cart_col.doc(item._id).remove({//删除对应的item数据
      success(res) {
        console.log("删除成功", res)
      }, fail(res) {
        console.log("删除失败", res)
      }
    })
    wx.showToast({
      title: '取消成功',
      icon: 'success',
      duration: 2500
    })
    this.onShow()
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    this.setData({
      uA: app.globalData.useraccount
    })
    let that = this
    wx.cloud.database().collection("cart").get({
      success(res) {
        console.log('成功', res.data)
        that.setData({
          datalist: res.data
        })
      }, fail(res) {
        console.log('失败', res)
      }
    })
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
    this.setData({
      uA: app.globalData.useraccount
    })
    let that = this
    wx.cloud.database().collection("cart").get({
      success(res) {
        console.log('成功', res.data)
        that.setData({
          datalist: res.data
        })
      }, fail(res) {
        console.log('失败', res)
      }
    })
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