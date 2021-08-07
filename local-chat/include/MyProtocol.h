#define PUBLIC_FIFO "publicFifo"
#define PUBLIC_FIFO_SIZE 4096

using namespace std;

struct MyProtocol
{
    /*
    pro_num:    协议号，1代表登陆请求包，2代表连接成功包，3代表数据发送包，4代表客户不在线包，5代表退出登陆包
    src:        发送方ID    
    dst:        接收方ID    
    data:       数据包
    */
    int pro_num;
    int src;
    int dst;
    char data[4084];
};
