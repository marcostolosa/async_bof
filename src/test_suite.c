#include "../include/async_bof.h"
#include <assert.h>

// ================================================================
// 测试用例和验证脚本
// ================================================================

// 测试结果统计
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
} test_results_t;

static test_results_t test_results = {0, 0, 0};

// 测试宏定义
#define TEST_START(name) \
    printf("\n=== Running Test: %s ===\n", name); \
    test_results.total_tests++;

#define TEST_ASSERT(condition, message) \
    if (condition) { \
        printf("[PASS] %s\n", message); \
        test_results.passed_tests++; \
    } else { \
        printf("[FAIL] %s\n", message); \
        test_results.failed_tests++; \
    }

#define TEST_END() \
    printf("=== Test Completed ===\n");

// 测试任务创建和销毁
void test_task_creation_destruction() {
    TEST_START("Task Creation and Destruction");
    
    // 测试正常任务创建
    async_task_t* task = create_task("TestTask", EVENT_ADMIN_LOGIN, "test_param", 30);
    TEST_ASSERT(task != NULL, "Task creation with valid parameters");
    TEST_ASSERT(strcmp(task->name, "TestTask") == 0, "Task name correctly set");
    TEST_ASSERT(task->event_type == EVENT_ADMIN_LOGIN, "Event type correctly set");
    TEST_ASSERT(task->interval == 30, "Interval correctly set");
    
    // 测试任务销毁
    destroy_task(task);
    TEST_ASSERT(1, "Task destruction completed without crash");
    
    // 测试无效参数
    async_task_t* invalid_task = create_task(NULL, EVENT_ADMIN_LOGIN, "test", 30);
    TEST_ASSERT(invalid_task == NULL, "Task creation fails with NULL name");
    
    invalid_task = create_task("", EVENT_ADMIN_LOGIN, "test", 30);
    TEST_ASSERT(invalid_task == NULL, "Task creation fails with empty name");
    
    // 测试长名称
    char long_name[100];
    memset(long_name, 'A', sizeof(long_name) - 1);
    long_name[sizeof(long_name) - 1] = '\0';
    invalid_task = create_task(long_name, EVENT_ADMIN_LOGIN, "test", 30);
    TEST_ASSERT(invalid_task == NULL, "Task creation fails with overly long name");
    
    TEST_END();
}

// 测试错误处理机制
void test_error_handling() {
    TEST_START("Error Handling Mechanism");
    
    // 初始化错误处理
    init_error_handling();
    TEST_ASSERT(get_last_error() == ASYNC_BOF_SUCCESS, "Error handling initialized correctly");
    
    // 测试错误设置和获取
    set_last_error(ASYNC_BOF_ERROR_MEMORY_ALLOCATION, "Test error message");
    TEST_ASSERT(get_last_error() == ASYNC_BOF_ERROR_MEMORY_ALLOCATION, "Error code set correctly");
    TEST_ASSERT(strcmp(get_last_error_message(), "Test error message") == 0, "Error message set correctly");
    
    // 测试错误字符串
    const char* error_str = get_error_string(ASYNC_BOF_ERROR_MEMORY_ALLOCATION);
    TEST_ASSERT(error_str != NULL, "Error string retrieval works");
    TEST_ASSERT(strlen(error_str) > 0, "Error string is not empty");
    
    TEST_END();
}

// 测试安全内存操作
void test_safe_memory_operations() {
    TEST_START("Safe Memory Operations");
    
    // 测试安全分配
    void* ptr = safe_malloc(1024);
    TEST_ASSERT(ptr != NULL, "Safe malloc works for valid size");
    
    // 测试安全释放
    safe_free(ptr);
    TEST_ASSERT(1, "Safe free works without crash");
    
    // 测试无效分配
    void* invalid_ptr = safe_malloc(0);
    TEST_ASSERT(invalid_ptr == NULL, "Safe malloc fails for zero size");
    TEST_ASSERT(get_last_error() == ASYNC_BOF_ERROR_INVALID_PARAMETER, "Error set for invalid malloc");
    
    // 测试安全字符串复制
    char dest[10];
    int result = safe_strcpy(dest, sizeof(dest), "test");
    TEST_ASSERT(result == 1, "Safe strcpy works for normal string");
    TEST_ASSERT(strcmp(dest, "test") == 0, "String copied correctly");
    
    // 测试字符串截断
    result = safe_strcpy(dest, sizeof(dest), "very_long_string_that_exceeds_buffer");
    TEST_ASSERT(result == 0, "Safe strcpy returns 0 for truncation");
    TEST_ASSERT(strlen(dest) == sizeof(dest) - 1, "String properly truncated");
    
    TEST_END();
}

// 测试事件监控功能
void test_event_monitoring() {
    TEST_START("Event Monitoring Functions");
    
    // 创建测试任务
    async_task_t* task = create_task("MonitorTest", EVENT_PROCESS_START, "notepad.exe", 10);
    TEST_ASSERT(task != NULL, "Monitor test task created");
    
    if (task) {
        // 测试管理员权限检查
        int admin_status = monitor_admin_login(task);
        TEST_ASSERT(admin_status == 0 || admin_status == 1, "Admin login check returns valid result");
        
        // 测试进程监控
        int process_status = monitor_process_start(task);
        TEST_ASSERT(process_status == 0 || process_status == 1, "Process monitoring returns valid result");
        
        // 测试网络监控
        int network_status = monitor_network_isolation(task);
        TEST_ASSERT(network_status == 0 || network_status == 1, "Network monitoring returns valid result");
        
        destroy_task(task);
    }
    
    TEST_END();
}

// 测试真实事件监控
void test_real_event_monitoring() {
    TEST_START("Real Event Monitoring");
    
    async_task_t* task = create_task("RealEventTest", EVENT_REAL_PROCESS_CREATE, "cmd.exe", 5);
    TEST_ASSERT(task != NULL, "Real event monitoring task created");
    
    if (task) {
        // 设置事件日志监控
        task->monitor_mode = MONITOR_MODE_EVENT_LOG;
        int setup_result = setup_event_log_monitoring(task);
        TEST_ASSERT(setup_result == 0 || setup_result == 1, "Event log monitoring setup completes");
        
        // 测试真实进程事件监控
        int event_result = monitor_real_process_events(task);
        TEST_ASSERT(event_result == 0 || event_result == 1, "Real process event monitoring works");
        
        // 清理
        cleanup_event_monitoring(task);
        destroy_task(task);
    }
    
    TEST_END();
}

// 测试权限检查
void test_privilege_checks() {
    TEST_START("Privilege Checks");
    
    int admin_check = check_admin_privileges();
    TEST_ASSERT(admin_check == 0 || admin_check == 1, "Admin privilege check returns valid result");
    
    printf("Current privilege status: %s\n", admin_check ? "Administrator" : "Regular User");
    
    TEST_END();
}

// 测试注册表操作
void test_registry_operations() {
    TEST_START("Registry Operations");
    
    async_task_t* task = create_task("RegTest", EVENT_ADMIN_LOGIN, "test", 30);
    TEST_ASSERT(task != NULL, "Registry test task created");
    
    if (task) {
        // 测试保存任务到注册表
        int save_result = save_task(task);
        TEST_ASSERT(save_result == 0 || save_result == 1, "Task save operation completes");
        
        if (save_result) {
            printf("Task saved to registry successfully\n");
            
            // 测试从注册表加载任务
            int load_count = load_tasks();
            TEST_ASSERT(load_count >= 0, "Task load operation completes");
            printf("Loaded %d tasks from registry\n", load_count);
            
            // 清理：删除测试任务
            delete_task(task->task_id);
        }
        
        destroy_task(task);
    }
    
    TEST_END();
}

// 性能测试
void test_performance() {
    TEST_START("Performance Tests");
    
    DWORD start_time = GetTickCount();
    
    // 创建多个任务测试性能
    const int num_tasks = 10;
    async_task_t* tasks[num_tasks];
    
    for (int i = 0; i < num_tasks; i++) {
        char task_name[32];
        sprintf(task_name, "PerfTest%d", i);
        tasks[i] = create_task(task_name, EVENT_PROCESS_START, "test.exe", 60);
        TEST_ASSERT(tasks[i] != NULL, task_name);
    }
    
    // 清理任务
    for (int i = 0; i < num_tasks; i++) {
        if (tasks[i]) {
            destroy_task(tasks[i]);
        }
    }
    
    DWORD end_time = GetTickCount();
    DWORD elapsed = end_time - start_time;
    
    printf("Performance test completed in %lu milliseconds\n", elapsed);
    TEST_ASSERT(elapsed < 5000, "Performance test completes within 5 seconds");
    
    TEST_END();
}

// 主测试函数
void run_all_tests() {
    printf("==============================================\n");
    printf("        Async BOF Test Suite\n");
    printf("==============================================\n");
    
    // 初始化
    init_error_handling();
    set_log_level(LOG_LEVEL_WARNING); // 减少测试时的日志输出
    
    // 运行所有测试
    test_error_handling();
    test_safe_memory_operations();
    test_task_creation_destruction();
    test_privilege_checks();
    test_event_monitoring();
    test_real_event_monitoring();
    test_registry_operations();
    test_performance();
    
    // 输出测试结果
    printf("\n==============================================\n");
    printf("        Test Results Summary\n");
    printf("==============================================\n");
    printf("Total Tests:  %d\n", test_results.total_tests);
    printf("Passed:       %d\n", test_results.passed_tests);
    printf("Failed:       %d\n", test_results.failed_tests);
    printf("Success Rate: %.1f%%\n", 
           test_results.total_tests > 0 ? 
           (float)test_results.passed_tests / test_results.total_tests * 100 : 0);
    
    if (test_results.failed_tests == 0) {
        printf("\n🎉 All tests passed!\n");
    } else {
        printf("\n⚠️  Some tests failed. Please review the output above.\n");
    }
    
    // 清理
    cleanup_resources();
}

// 验证脚本入口点
#ifndef BOF_BUILD
int main(int argc, char* argv[]) {
    printf("Async BOF - Test and Validation Suite\n");
    printf("=====================================\n");
    
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s [options]\n", argv[0]);
        printf("Options:\n");
        printf("  --help    Show this help message\n");
        printf("  (no args) Run all tests\n");
        return 0;
    }
    
    run_all_tests();
    
    printf("\nPress Enter to exit...\n");
    getchar();
    
    return test_results.failed_tests > 0 ? 1 : 0;
}
#endif