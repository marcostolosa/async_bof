# 🚀 Async BOF 生产部署指南

## 📋 部署清单

### 系统要求
- Windows 10/11 或 Windows Server 2019/2022
- Cobalt Strike 4.0+ 
- 适当的系统权限（取决于监控类型）

### 构建要求
- x86_64-w64-mingw32-gcc（用于BOF编译）
- 标准GCC（用于本地测试）

## 🛠️ 快速部署

### 1. 构建生产版本
```bash
# 编译所有组件
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -O2 -o bin/async_bof.o src/async_bof.c
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -O2 -o bin/real_event_monitor.o src/real_event_monitor.c  
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -O2 -o bin/error_handling.o src/error_handling.c

# 创建合并的BOF文件
ld -r -o bin/async_bof_combined.o bin/async_bof.o bin/real_event_monitor.o bin/error_handling.o
```

### 2. 部署到Cobalt Strike
```bash
# 复制到CS目录
cp bin/async_bof_combined.o /path/to/cobaltstrike/
cp async_bof.cna /path/to/cobaltstrike/
```

### 3. 加载到CS
1. 打开Cobalt Strike客户端
2. Script Manager → Load → 选择 `async_bof.cna`
3. 脚本加载成功后会显示确认信息

## 🎯 使用指南

### 图形界面操作

#### 添加监控任务
1. **右键点击Beacon** → `异步BOF` → `添加监控器`
2. **选择监控类型**：
   - 管理员登录监控
   - 进程启动监控  
   - 用户登录监控
   - 网络隔离监控
   - 剪贴板监控
   - 键盘监控
   - 等等...

3. **配置参数**：
   - **监控目标**：输入具体参数（如进程名、用户名）
   - **检查间隔**：设置监控频率（秒）
   - **监控模式**：选择轮询或事件驱动

4. **点击添加**创建任务

#### 管理现有任务
- **查看任务**：右键 → `异步BOF` → `列出任务`
- **取消任务**：右键 → `异步BOF` → `取消任务` → 输入任务ID
- **删除任务**：右键 → `异步BOF` → `删除任务` → 输入任务ID

### 命令行操作

#### 基本命令
```bash
# 加载现有任务
async_bof

# 添加任务（语法：名称 类型 参数 间隔）
async_bof_add_task "AdminWatcher" 0 "admin" 30
async_bof_add_task "ProcessMon" 1 "notepad.exe" 60  
async_bof_add_task "LoginMon" 2 "administrator" 45

# 管理任务
async_bof_list                    # 列出所有任务
async_bof_cancel 1                # 取消任务ID为1的任务
async_bof_delete 2                # 删除任务ID为2的任务
```

#### 事件类型对照表
| 类型ID | 事件名称 | 描述 | 示例参数 |
|--------|----------|------|----------|
| 0 | 管理员登录 | 检测管理员权限获取 | "admin" |
| 1 | 进程启动 | 监控特定进程启动 | "notepad.exe" |
| 2 | 用户登录 | 监控用户登录事件 | "administrator" |
| 3 | 网络隔离 | 检测网络连接断开 | "" |
| 4 | VPN连接 | 监控VPN状态 | "" |
| 5 | WiFi连接 | 监控WiFi状态 | "WorkWiFi" |
| 6 | 票据过期 | Kerberos票据预警 | "60" |
| 7 | 剪贴板 | 监控剪贴板内容 | "password" |
| 8 | 键盘输入 | 监控键盘活动 | "" |
| 9 | 用户登录事件 | 增强登录监控 | "domain\\user" |

## ⚙️ 高级配置

### 真实事件监控

#### 启用审计策略
```powershell
# 启用进程跟踪
auditpol /set /category:"Detailed Tracking" /subcategory:"Process Creation" /success:enable

# 启用登录审计  
auditpol /set /category:"Logon/Logoff" /subcategory:"Logon" /success:enable /failure:enable

# 启用文件系统审计
auditpol /set /category:"Object Access" /subcategory:"File System" /success:enable

# 启用注册表审计
auditpol /set /category:"Object Access" /subcategory:"Registry" /success:enable
```

#### 权限要求
- **基本监控**：用户权限
- **事件日志监控**：SeAuditPrivilege
- **ETW监控**：管理员权限
- **高级监控**：SeDebugPrivilege

### 监控模式选择

#### 轮询模式 (默认)
```bash
# 适用于大部分场景，权限要求低
async_bof_add_task "BasicMon" 1 "cmd.exe" 30
```

#### 事件日志模式
```bash  
# 需要在Aggressor脚本中配置监控模式
# 或通过真实事件监控功能使用
```

## 🔧 故障排除

### 常见问题

#### 1. 脚本加载失败
**症状**：Aggressor脚本无法加载
**解决**：
- 检查BOF文件是否在正确位置
- 确认文件权限
- 查看CS控制台错误信息

#### 2. 任务创建失败
**症状**：任务无法创建或立即失败
**解决**：
- 检查参数格式是否正确
- 确认目标系统权限
- 查看任务状态和错误信息

#### 3. 事件未触发
**症状**：监控运行但未检测到事件
**解决**：
- 确认监控条件确实发生
- 检查监控间隔设置
- 验证权限是否足够
- 查看日志输出

#### 4. 性能问题
**症状**：系统响应缓慢或CPU使用率高
**解决**：
- 增加监控间隔
- 减少同时运行的任务数
- 使用事件驱动模式替代轮询

### 调试选项

#### 启用详细日志
```bash
# 在测试环境中运行
bin/async_bof_test.exe
```

#### 检查任务状态
```bash
# 通过列出任务查看详细状态
async_bof_list
```

## 📊 性能调优

### 资源使用监控
- **内存使用**：每个任务 < 2MB
- **CPU使用**：轮询模式 < 1%，事件模式 < 0.1%
- **网络流量**：最小化，仅在事件触发时通信

### 优化建议

#### 任务数量
- **推荐**：单个Beacon ≤ 10个并发任务
- **最大**：根据系统资源调整

#### 监控间隔
- **高优先级事件**：10-30秒
- **一般监控**：60-300秒  
- **后台监控**：600秒或更长

#### 监控模式选择
- **实时需求**：使用事件日志模式
- **资源受限**：使用轮询模式
- **高权限环境**：使用ETW模式

## 🛡️ OPSEC 最佳实践

### 隐匿性考虑

#### 监控间隔随机化
```bash
# 使用不规则间隔避免模式识别
async_bof_add_task "StealthMon" 1 "cmd.exe" 45   # 而非30的倍数
```

#### 分散任务分布
- 避免同时创建大量任务
- 使用不同的检查间隔
- 适当延迟任务启动

#### API调用最小化
- 优先使用事件驱动模式
- 合理设置监控间隔
- 避免不必要的重复检查

### 权限管理
- 使用最小必要权限
- 避免不必要的管理员权限请求
- 合理利用现有权限

## 📚 参考资料

### 相关文档
- [Cobalt Strike BOF 开发指南](https://hstechdocs.helpsystems.com/manuals/cobaltstrike/current/userguide/content/topics/beacon-object-files_main.htm)
- [Windows 事件日志参考](https://docs.microsoft.com/en-us/windows/security/threat-protection/auditing/security-auditing-overview)
- [ETW 开发文档](https://docs.microsoft.com/en-us/windows/desktop/etw/event-tracing-portal)

### 社区资源
- GitHub Issues: 问题报告和功能请求
- 示例配置文件
- 扩展监控模块

---

**部署完成后，建议先在测试环境验证所有功能正常，再部署到生产环境。**

**🎉 祝您使用愉快！如有问题请及时反馈。**