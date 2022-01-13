// components/weekch/weekch.js
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
    array: ['第1周', '第2周', '第3周', '第4周', '第5周', '第6周', '第7周', '第8周', '第9周', '第10周', '第11周', '第12周', '第13周', '第14周', '第15周', '第16周'],
    objectArray: [
      {
        id: 0,
        name: '第1周'
      },
      {
        id: 1,
        name: '第2周'
      },
      {
        id: 2,
        name: '第3周'
      },
      {
        id: 3,
        name: '第4周'
      },
      {
        id: 4,
        name: '第5周'
      },
      {
        id: 5,
        name: '第6周'
      },
      {
        id: 6,
        name: '第7周'
      },
      {
        id: 7,
        name: '第8周'
      },
      {
        id: 8,
        name: '第9周'
      },
      {
        id: 9,
        name: '第10周'
      },
      {
        id: 10,
        name: '第11周'
      },
      {
        id: 11,
        name: '第12周'
      },
      {
        id: 12,
        name: '第13周'
      },
      {
        id: 13,
        name: '第14周'
      },
      {
        id: 14,
        name: '第15周'
      },
      {
        id: 15,
        name: '第16周'
      }
    ],
    index:0

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
      var data= e.detail.value
      this.triggerEvent('weekdata',data)
    }
  }


})
