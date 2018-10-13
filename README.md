# **K6xNetworkDemo**
This project demonstrate connect and use network via Ethernet or SIM Module using Freescale K6x MCU 

## **Hardware**
<p align="center"> <img src="./images/HW.png"></p>
- Network module sử dụng ethernet là datalink chính và một đường SIM (wireless 3G) là backup khi mất kết nối ethernet

## **Software**
<p align="center"> <img src="./images/Stack.png"></p>
- Module network giao tiếp với máy tính cấu hình (máy tính là TCP client, logger là TCP server)

- Giao tiếp với FTP-server để log file dữ liệu

- Giao tiếp với TCP-Server để gửi và nhận lệnh

**Yêu cầu chung của module network:**

+ Có file network\_cfg.h để cấu hình các thông số của hệ thống:

+ Tích hợp để sử dụng trong môi trường RTOS (uCOSIII)

+ Sử dụng non-blocking call

+ Có các hàm callback khi có sự kiện nhận, sự kiện lỗi, sự kiện gửi thành công

+ Là module độc lập trong project

+ Hỗ trợ hai loại socket UDP và TCP

+ Hỗ trợ SSL

+ Khi đường ethernet mất kết nối (retry connect 10 lần mỗi lần cách nhau 1 giây mà không kết nối được), tự động chuyển sang đường wireless backup

+ Khi  đang sử dụng đường wireless backup mà phát hiện có connection trên ehternet thì sử dụng đường kết nối ethernet.

+ Mất kết nối thì đồng thời retry connect trên cả hai module ehternet và wireless cho đến khi ít nhất một trong hai đường kết nối được thì dừng lại, sử dụng đường kết nối thành công, nếu cả hai đường kết nối thành công thì ưu tiên sử dụng ethernet

**TCP Client:**

+ Kết nối đến server và truyền nhận các bản tin

**TCP Server:**

+ Nhận kết nối từ client, giới hạn số kết nối tối đa bằng 5, truyền nhận các bản tin với client

**FTP Client:**

+ Kết nối, authenticate với FTP-Server chuẩn, passive mode

+ Tạo thư mục, di chuyển đến thư mục, truyền file vào thư mục

+ Xóa thư mục, xóa file

+ Tự động kiểm tra những file còn thiếu, từ thẻ nhớ so với FTP-Server và đồng bộ các file còn thiếu đó khi có mạng trở lại

+ Các thông số cấu hình chung của module:

| **STT** | **Thông số cấu hình** | **Giá trị** | **Ý nghĩa** |
| --- | --- | --- | --- |
| 1 | NET\_COM\_USING\_HW | ETHERNET/SIM | Cấu hình sử dụng network hw |
| 2 | NET\_COM\_IP\_METHOD | DHCP/STATIC | Cấu hình sử dụng dhcp hay static ip |
| 3 | NET\_COM\_RECV\_BUFF | 1024\*10 | Cấu hình độ lớn buffer truyền và buffer nhận |
| 4 | NET\_COM\_SEND\_BUFF | 1024\*10 |   |
| 5 | NET\_COM\_FTP\_USER |   |   |
| 6 | NET\_COM\_FTP\_PASSWD |   |   |

**Các API external của module** :
<table>
<tr> <th> STT</th>
<th> Hàm API cơ bản </th>
<th> Ý nghĩa </th> </tr>
<tr> 
    <td> 1 </td> 
    <td> 
        <strong>Mã lỗi cơ bản:</strong> <br>
        <code>
        enum NetStatus { 
        <p style ="margin-left:40px">
        NET_ERR_NONE = 0, <br/>  NET_ERR_MODULE_NOT_INIT, <br/>
        NET_ERR_PARAM,  <br>
        NET_ERR_SEND_FAILD, <br>
        NET_ERR_LOST_CONNECTION, <br>
        NET_ERR_BUSY, <br>
        NET_ERR_UNKNOWN <br> </p>
        };
        </code> 
    </td>
</tr>
<tr> 
    <td>2</td>
    <td><code/>int Net_ModuleInitHw();</td>
    <td></td>
</tr>
<tr> 
    <td>3</td>
    <td><code/>NetStatus Net_TCPServerStart(int port);</td>
    <td>Bắt đầu TCPClient kết nối đến server địa chỉ ip:port </td>
</tr>
<tr> 
    <td>4</td>
    <td><code/>NetStatus Net_TCPClientStart(const char* ip, int port);</td>
    <td>Bắt đầu TCPClient kết nối đến server địa chỉ ip:port </td>
</tr>
<tr> 
    <td>5</td>
    <td><code/>SNetStt Net_ModuleGetStatus();</td>
    <td> Trả về cấu trúc trạng thái của module network:
        <ul>
        <li>
Đang sử dụng đường hw nào
           Ethernet,
           WireLess
        </li>
        <li>
        Trạng thái kết nối:
                    Connected,
                    Disconnected,
                    Error
        </li>
        </ul>
 </td>
</tr>
<tr> 
    <td>6</td>
    <td><code/>NetStatus Net_FTPClientStart(const char *ip, int port, const char* usrname, const char* passwd);</td>
    <td> Start FTP client với địa chỉ, port, username passwd cho trước, sử dụng mode passive</td>
</tr>
<tr> 
    <td>7</td>
    <td><code/>NetStatus Net_FTPClientSendFile(const char *dirPath, const char *fileName);</td>
    <td>Gửi file lên FTP server vào thư mục dirPath, nếu thư mục dirPath chưa có thì tạo thư mục đó (trên FTP-server), trả về mã lỗi sau khi thực hiện</td>
</tr>
<tr> 
    <td>8</td>
    <td><code/>NetStatus Net_TCPClientSendCmd(const uint8_t *data, uint32_t length);</td>
    <td>Gửi dữ liệu cho server</td>
</tr>
<tr> 
    <td>9</td>
    <td><code/>NetStatus Net_TCPServerSendCmdToAllClient(const uint8_t *data, uint32_t length);</td>
    <td>Gửi dữ liệu cho toàn bộ các client kết nối đến server</td>
</tr>
</table>

+ Người viết phần mềm có thể tự thêm các API bổ sung để tiện quản lý và sử dụng module

**Yêu cầu riêng**

1. Module ethernet:

+ Sử dụng ethernet MAC của MCU

+ Sử dụng DMA

2. Module wireless:

+ Sử dụng module 3G của Simcom

+ Recommend sử dụng tập lệnh AT

support framework-sample: có trong SDK của hãng:

[https://www.nxp.com/support/developer-resources/evaluation-and-development-boards/freedom-development-boards/mcu-boards/software-development-kit-for-kinetis-mcus:KINETIS-SDK](https://www.nxp.com/support/developer-resources/evaluation-and-development-boards/freedom-development-boards/mcu-boards/software-development-kit-for-kinetis-mcus:KINETIS-SDK)