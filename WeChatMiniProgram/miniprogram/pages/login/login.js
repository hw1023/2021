// pages/login/login.js
var app = getApp();
// 获取云数据库引用
const db = wx.cloud.database();
const users = db.collection('user');//注意，这里就是刚才的集合的名字——user
let name = null;//变量，用于存放用户输入的账号
let password = null;//变量，用于存放用户输入的密码

Page({

  /**
   * 页面的初始数据
   */
  data: {

  },
  //输入用户名
  inputName: function (event) {
    name = event.detail.value//将用户输入的账号放到变量里面
  },
  //输入密码
  inputPassword(event) {
    password = event.detail.value//将用户输入的密码放到变量里面
  },
  //登陆函数
  login() {
    let that = this;
    //登陆获取用户信息
    users.get({
      success: (res) => {
        let user = res.data;
        console.log(res.data);
        for (let i = 0; i < user.length; i++) {  //遍历数据库对象集合
          if (name == user[i].account) { //用户名存在
            if (password == user[i].password) {  //判断密码是否正确
              if(name == 'admin'){
                wx.showToast({
                  title: '登录成功',
                  icon: 'success'
                })
                wx.redirectTo({
                  url: '/pages/admin/admin'//这里是管理员成功登录后跳转的页面
                })
                break;
              };
              wx.showToast({
                title: '登录成功',
                icon: 'success',
                duration:2500
              })
              console.log('登陆成功！')
              app.globalData.useraccount = user[i].account
              app.globalData.username = user[i].name
              app.globalData.userpassword = user[i].password
              app.globalData.usertype = user[i].type
              app.globalData.userid = user[i]._id
              wx.switchTab({
                url: '/pages/myinfo/myinfo'//这里是教师和学生成功登录后跳转的页面
              })
              break;
            } else if (password !== user[i].password){
              console.log('password wrong')
              wx.showToast({
                title: '用户名或密码错误',
                icon: 'none'
              })
            }
          } else if (name !== user[i].account&&i==user.length-1){
              console.log('wrong!')
              wx.showToast({
                title: '用户名或密码错误',
                icon: 'none'
              })
             }       
          
        }
      }
      
    })
  }
})