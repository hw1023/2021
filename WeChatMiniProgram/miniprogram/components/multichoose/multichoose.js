// components/multichoose/multichoose.js
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
    index:0,
    multiArray: [['空闲', '有课'], ['图东教学楼', '图西教学楼', '商学院', '海洋学院', '图西环楼'], ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节']],
    objectMultiArray: [
      [
        {
          id: 0,
          name: '空闲'
        },
        {
          id: 1,
          name: '有课'
        }
      ], [
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
      ], [
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
      ]
    ],
    multiIndex: [0, 0, 0]
  },

  /**
   * 组件的方法列表
   */
  methods: {

    bindMultiPickerChange: function (e) {
      console.log('picker发送选择改变，携带值为', e.detail.value)
      this.setData({
        multiIndex: e.detail.value
      })
      this.triggerEvent('detail', e.detail.value)
    },


    
    bindMultiPickerColumnChange: function (e) {
      console.log('修改的列为', e.detail.column, '，值为', e.detail.value);
      var data = {
        multiArray: this.data.multiArray,
        multiIndex: this.data.multiIndex
      };
      data.multiIndex[e.detail.column] = e.detail.value;
      switch (e.detail.column) {
        case 0:
          switch (data.multiIndex[0]) {
            case 0:
              data.multiArray[1] = ['图东教学楼', '图西教学楼', '商学院', '海洋学院', '图西环楼'];
              data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
              break;
            case 1:
              data.multiArray[1] = ['图东教学楼', '图西教学楼', '商学院', '海洋学院', '图西环楼'];
              data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
              break;
          }
          data.multiIndex[1] = 0;
          data.multiIndex[2] = 0;
          break;
        case 1:
          switch (data.multiIndex[0]) {
            case 0:
              switch (data.multiIndex[1]) {
                case 0:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
                case 1:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
                case 2:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
                case 3:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
                case 4:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
              }
              break;
            case 1:
              switch (data.multiIndex[1]) {
                case 0:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
                case 1:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
                case 2:
                  data.multiArray[2] = ['第一大节', '第二大节', '第三大节', '第四大节', '第五大节', '第六大节'];
                  break;
              }
              break;
          }
          data.multiIndex[2] = 0;
          console.log(data.multiIndex);
          break;
      }
      this.setData(data);
    }
  }
})
