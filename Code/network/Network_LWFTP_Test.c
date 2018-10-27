/**
 * @file Network_LWFTP_Test.c
 * @author ThinhNT (tienthinh@gmail.com)
 * @brief Simple test for Network_LWFTP Module
 * @version 0.1
 * @date 2018-10-17
 *
 * @copyright Copyright (c) 2018
 *
 */


#include "Network_LWFTP_Test.h"


static void ftp_retr_callback(void *arg, int result)
{
    lwftp_session_t *s = (lwftp_session_t*)arg;

    if ( result != LWFTP_RESULT_OK ) {
        LOG_ERROR("retr failed (%d)", result);
        return Network_LWFTP_Close(s);
    }
    // Test is done
    Network_LWFTP_Close(s);
}

static uint data_sink(void *arg, const char* ptr, uint len)
{
    static const uint mylen = 12345;
    static char * const myconfig = (char*)0x20000000;
    static uint offset = 0;

    if (ptr) {
        len = min( len, mylen-offset );
        memcpy( myconfig+offset, ptr, len );
        offset += len;
    }
    return len;
}

static void ftp_stor_callback(void *arg, int result)
{
    lwftp_session_t *s = (lwftp_session_t*)arg;
    err_t error;

    if ( result != LWFTP_RESULT_OK ) {
        LOG_ERROR("stor failed (%d)", result);
        return Network_LWFTP_Close(s);
    }
    // Continue with RETR request
    s->data_sink = data_sink;
    s->done_fn = ftp_retr_callback;
    s->remote_path = "configfile";
    error = Network_LWFTP_Retrieve(s);
    if ( error != LWFTP_RESULT_INPROGRESS ) {
        LOG_ERROR("lwftp_retrieve failed (%d)", error);
    }
    // FTP session will continue with RETR and sink callbacks
}

static uint data_source(void *arg, const char** pptr, uint maxlen)
{
    static const uint mylen = 12345;
    static const char * const mydata = (char*)0x20000000;
    static uint offset = 0;
    uint len = 0;

    // Check for data request or data sent notice
    if (pptr) {
        len = mylen - offset;
        if ( len > maxlen ) len = maxlen;
        *pptr = mydata + offset;
    } else {
        offset += maxlen;
        if ( offset > mylen ) offset = mylen;
    }
    return len;
}

static void ftp_connect_callback(void *arg, int result)
{
    lwftp_session_t *s = (lwftp_session_t*)arg;
    err_t error;

    if ( result != LWFTP_RESULT_LOGGED ) {
        LOG_ERROR("login failed (%d)", result);
        return Network_LWFTP_Close(s);
    }
    // Continue with STOR request
    s->data_source = data_source;
    s->done_fn = ftp_stor_callback;
    s->remote_path = "logfile";
    error = Network_LWFTP_Store(s);
    if ( error != LWFTP_RESULT_INPROGRESS ) {
        LOG_ERROR("lwftp_store failed (%d)", error);
    }
    // FTP session will continue with STOR and source callbacks
}

void Network_LWFTP_Test(void)
{
    static lwftp_session_t s;   // static content for the whole FTP session
    err_t error;
    PRINTF("Start Network_LWFTP_Test\r\n");
    // Initialize session data
    memset(&s, 0, sizeof(s));
    IP4_ADDR(&s.server_ip, 10,2,82,89);
    s.server_port = 21;
    s.done_fn = ftp_connect_callback;
    s.user = "thinhnt";
    s.pass = "123456a@";
    // We have no extra user data, simply use the session structure
    s.handle = &s;

    // Start the connection state machine
    error = Network_LWFTP_Connect(&s);
    if ( error != LWFTP_RESULT_INPROGRESS ) {
        LOG_ERROR("lwftp_connect failed (%d)", error);
    }
    // FTP session will continue with the connection callback
}
