// components/time/time.js
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
    array: ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'],
    objectArray: [
      {
        id: 0,
        name: '第一大节'
      },
      {
        id: 1,
        name: '第二大节'
      },
      {
        id: 2,
        name: '第三大节'
      },
      {
        id: 3,
        name: '第四大节'
      },
      {
        id: 4,
        name: '第五大节'
      },
      {
        id: 5,
        name: '第六大节'
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
      this.triggerEvent('timedata', data)
    }
  }
})
