// components/day/day.js
Component({
  /**
   * 组件的属性列表
   */
  properties: {

  },

  /**
   * 组件的初始数据
   */
  data: {
    array: ['星期一', '星期二', '星期三', '星期四', '星期五', '星期六','星期日'],
    objectArray: [
      {
        id: 0,
        name: '星期一'
      },
      {
        id: 1,
        name: '星期二'
      },
      {
        id: 2,
        name: '星期三'
      },
      {
        id: 3,
        name: '星期四'
      },
      {
        id: 4,
        name: '星期五'
      },
      {
        id: 5,
        name: '星期六'
      },
      {
        id: 6,
        name: '星期日'
      }
    ],
    index: 0
  },

  /**
   * 组件的方法列表
   */
  methods: {
    bindPickerChange: function (e) {
      console.log('picker发送选择改变，携带值为', e.detail.value)
      console.log(e)
      this.setData({
        index: e.detail.value
      })
      var data = e.detail.value
      this.triggerEvent('daydata', data)
    }
  }
})
