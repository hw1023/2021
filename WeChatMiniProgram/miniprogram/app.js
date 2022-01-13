//app.js
App({
globalData:{
  useraccount:null,
  username: null,
  usertype:null,
  userpassword:null,
  userid:null
},

  onLaunch: function () {
    wx.cloud.init({
      env: 'wayn-cotgo'
    }),
    this.globalData.useraccount='default'
    this.globalData.username = 'default'
    this.globalData.usertype = 'default'
    this.globalData.userpassword = 'default'
    this.globalData.userid = 'default'
  }
})
