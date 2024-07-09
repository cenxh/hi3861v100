// index.js
const app = getApp();
let Utf8ArrayToStr = require('Utf8ArrayToStr/Utf8ArrayToStr.js');

Page({
  data: {
    udpResData: '',
    lampStatusText: '灯已打开', // 初始灯状态文本
    classroom1: {
      temperature: '0.00',
      occupied: false,
      airConditioner: false
    },
    classroom2: {
      temperature: '0.00',
      occupied: false,
      airConditioner: false
    }
  },
  classroomTap: function(e) {
    // 从按钮的 data-classroomId 属性获取教室编号
    const classroomNumber = e.currentTarget.dataset.classroomid; // 这应该是 "1" 或 "2"
    // 构建 data 对象的键名
    const classroomKey = `classroom${classroomNumber}`;

    // 读取当前教室的 isInSession 状态
    const currentIsInSession = this.data[classroomKey].isInSession;
    // 切换状态
    const newIsInSession = !currentIsInSession;

    // 更新数据
    this.setData({
      [classroomKey]: {
        ...this.data[classroomKey],
        isInSession: newIsInSession ? '上课中' : '未上课',
      }
    });

    // 发送上课状态信息
    this.sendMessage(classroomNumber, newIsInSession ? '上课中' : '未上课');
  },

  // 绑定切换占用状态按钮的点击事件
  toggleOccupiedStatus: function(e) {
    const classroomNumber = e.currentTarget.dataset.classroomid;
    const classroomKey = `classroom${classroomNumber}`;
    const currentOccupied = this.data[classroomKey].occupied;

    // 切换占用状态
    const newOccupied = !currentOccupied;

    // 更新数据
    this.setData({
      [classroomKey]: {
        ...this.data[classroomKey],
        occupied: newOccupied,
      }
    });

    // 发送占用状态信息
    this.sendMessage(classroomNumber, newOccupied ? '占用中' : '空闲');
  },

  sendMessage: function(e) {
    if (this.udp) {
      this.udp.send({
        address: '192.168.43.25',
        port: 8888,
        message: '请求教室信息' // 发送请求以获取教室信息
      });
    } else {
      console.error('UDP socket is not initialized.');
    }
  },

  onUdpMessage: function(res) {
    // 处理UDP消息，更新教室状态
    // 这里需要根据实际的消息格式来解析数据
    // 示例：接收到的数据格式 "教室1温度,占用情况,空调状态;教室2温度,占用情况,空调状态"
    let messageStr = Utf8ArrayToStr.Utf8ArrayToStr(new Uint8Array(res.message));
    let classroomsData = messageStr.split(';');

    if (classroomsData.length > 0) {
      let classroom1Data = classroomsData[0].split(',');
      this.setData({
        classroom1: {
          temperature: classroom1Data[0],
          occupied: classroom1Data[1] === '占用',
          airConditioner: classroom1Data[2] === '开启'
        }
      });
    }

    if (classroomsData.length > 1) {
      let classroom2Data = classroomsData[1].split(',');
      this.setData({
        classroom2: {
          temperature: classroom2Data[0],
          occupied: classroom2Data[1] === '占用',
          airConditioner: classroom2Data[2] === '开启'
        }
      });
    }
    // 更新接收内容
    this.setData({
      udpResData: 'UDP 接收到的内容: ' + messageStr
    });
  },

  onLoad: function() {
    // 新建udp实例
    this.udp = wx.createUDPSocket();

    // 绑定本机端口
    this.udp.bind({
      port: 9000, // 选择一个未被占用的端口
      address: '0.0.0.0', // 监听所有地址
      success: function() {
        console.log('UDP 绑定成功');
      },
      fail: function() {
        console.error('UDP 绑定失败');
      }
    });

    // 监听收到消息的事件
    this.udp.onMessage(this.onUdpMessage);

    console.log('页面 index 加载完成事件onLoad()');
  },

  onUnload: function() {
    // 清理udp资源
    if (this.udp) {
      this.udp.close();
      this.udp = null;
    }
  }
});