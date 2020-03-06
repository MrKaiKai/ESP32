
#ifndef __TCP_BSP_H__
#define __TCP_BSP_H__

#include <errno.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (!FALSE)
#endif

//#define TCP_SERVER_CLIENT_OPTION FALSE              //esp32作为client
#define TCP_SERVER_CLIENT_OPTION TRUE              //esp32作为server

//#define TAG                     "example_asr_keywords"           //打印的tag

//server
//AP热点模式的配置信息
#define SOFT_AP_SSID            "my_wifi"     //账号
#define SOFT_AP_PAS             "12345678"          //密码，可以为空
#define SOFT_AP_MAX_CONNECT     2                   //最多的连接点

//client
//STA模式配置信息,即要连上的路由器的账号密码
#define GATEWAY_SSID            "my_wifi"         //账号
#define GATEWAY_PAS             "12345678"       //密码
#define TCP_SERVER_ADRESS       "192.168.31.181"     //作为client，要连接TCP服务器地址
#define TCP_PORT                8086               //统一的端口号，包括TCP客户端或者服务端
#define TCP_PORT1               8080
//注：该IP地址视服务器而定，端口号0～65535


// FreeRTOS event group to signal when we are connected to wifi
#define WIFI_CONNECTED_BIT BIT0

#define WIFI_TC0 BIT1
#define WIFI_TC1 BIT2

extern EventGroupHandle_t tcp_event_group;

extern bool flag_con;
extern bool flag_con1;


extern bool g_rxtx_need_restart;
extern bool g_rxtx_need_restart1;                       //异常后，重新连接标记

extern int connect_socket;                          //连接socket
extern int connect_socket1;

//using esp as station
void wifi_init_sta();
//using esp as softap
void wifi_init_softap();

//create a tcp server socket. return ESP_OK:success ESP_FAIL:error
esp_err_t create_tcp_server(bool isCreatServer,int* socket_s,int* socket_c,int port);
//create a tcp client socket. return ESP_OK:success ESP_FAIL:error
esp_err_t create_tcp_client();

// //send data task
// void send_data(void *pvParameters);
//receive data task
void recv_data(void *pvParameters);
void recv_data1(void *pvParameters);
void send_data(void *pvParameters);
//close all socket
void close_socket();
void close_socket1();

//get socket error code. return: error code
int get_socket_error_code(int socket);

//show socket error code. return: error code
int show_socket_error_reason(const char* str, int socket);

//check working socket
int check_working_socket();

void tcp_bsp_init(void);

#ifdef __cplusplus
}
#endif


#endif /*#ifndef __TCP_BSP_H__*/

