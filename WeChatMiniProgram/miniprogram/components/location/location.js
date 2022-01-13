// components/location/location.js
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
    array: ['图东教学楼', '图西教学楼', '商学院', '海洋学院', '图西环楼'],
    objectArray: [
      {
        id: 0,
        name: '图东教学楼'
      },
      {
        id: 1,
        name: '图西教学楼'
      },
      {
        id: 2,
        name: '商学院'
      },
      {
        id: 3,
        name: '海洋学院'
      },
      {
        id: 4,
        name: '图西环楼'
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
      this.triggerEvent('locationdata', data)
    }
  }
})
