// pages/home/home.js
var w=0;
var l=0;
var t=0;
var d=0;
Page({
  data: {
    week:0,
    location:0,
    time:0,
    day:0
  },
click(){
  wx.navigateTo({
    url: '/pages/result/result?week=' + w + '&day=' + d + '&time=' + t + '&location=' + l
  })
},
getweekdata:function(e){
  console.log('1我拿到了detail',e.detail),
  this.setData({
    week:e.detail
  })
  w=e.detail
},
getlocation:function(e){
  console.log('2我拿到了location',e.detail)
  this.setData({
    location:e.detail
  })
  l=e.detail
},
gettime: function (e) {
  console.log('3我拿到了time', e.detail)
  this.setData({
     time: e.detail
  })
  t=e.detail
},
getday:function(e){
  console.log('3我拿到了time', e.detail)
  this.setData({
    day: e.detail
  })
  d = e.detail
}

})