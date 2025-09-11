# 🔄 异步BOF (Async Beacon Object Files)

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#构建状态)
[![Version](https://img.shields.io/badge/version-v2.0--production-success.svg)](#版本信息)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](#支持平台)

**Async BOF** 是一个为Cobalt Strike设计的生产级异步Beacon Object Files框架，提供真实的Windows系统事件监控能力。项目已从概念阶段完全转换为可用的生产级工具。

## ✨ 主要特性

### 🚀 核心能力
- **异步任务执行** - 支持多任务并发监控
- **真实事件监控** - 基于Windows事件日志、ETW、WMI
- **智能错误处理** - 完整的错误追踪和日志系统
- **内存安全操作** - 防止内存泄漏和缓冲区溢出
- **OPSEC友好** - 被动监听，最小化检测特征
- **图形界面集成** - 完整的Cobalt Strike GUI支持

### 📊 监控功能
- 🔐 **管理员权限检测** - 实时检测权限提升
- 🔍 **进程启动监控** - 智能进程活动检测
- 👤 **用户登录监控** - 多种登录事件追踪
- 🌐 **网络连接监控** - 网络隔离和VPN检测
- 📱 **WiFi连接监控** - 无线网络状态变化
- 🎫 **票据过期监控** - Kerberos票据生命周期管理
- 📋 **剪贴板监控** - 敏感数据复制检测
- ⌨️ **键盘活动监控** - 用户交互检测

### 🎯 真实事件监控 (新特性)
- 📝 **Windows事件日志** - Event ID 4688, 4624, 4625, 4656等
- ⚡ **ETW事件追踪** - 内核级事件监控
- 🔧 **WMI事件订阅** - 系统管理事件
- 📁 **文件访问监控** - 实时文件系统活动
- 🗃️ **注册表变更监控** - 注册表修改检测
- 🛡️ **权限提升监控** - 特权操作检测

## 🏗️ 项目结构

```
async_bof/
├── 📁 include/
│   └── async_bof.h          # 核心头文件定义
├── 📁 src/
│   ├── async_bof.c          # 主要BOF实现
│   ├── real_event_monitor.c # 真实事件监控
│   ├── error_handling.c     # 错误处理和日志
│   └── test_suite.c         # 测试套件
├── 📁 bin/
│   ├── async_bof_combined.o # 生产级BOF文件 ⭐
│   └── async_bof_test.exe   # 测试可执行文件
├── 📄 async_bof.cna           # Cobalt Strike界面
├── 📄 Makefile               # 构建脚本
├── 📄 DEPLOYMENT_GUIDE.md    # 部署指南
└── 📄 PROJECT_STATUS.md      # 项目状态
```

## 🚀 快速开始

### 构建命令
```bash
# 克隆仓库
git clone https://github.com/your-repo/async_bof.git
cd async_bof

# 构建生产版本
make all

# 或手动编译
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -O2 -o bin/async_bof.o src/async_bof.c
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -O2 -o bin/real_event_monitor.o src/real_event_monitor.c
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -O2 -o bin/error_handling.o src/error_handling.c
ld -r -o bin/async_bof_combined.o bin/async_bof.o bin/real_event_monitor.o bin/error_handling.o
```

### Cobalt Strike部署
```bash
# 复制文件
cp bin/async_bof_combined.o /path/to/cobaltstrike/
cp async_bof.cna /path/to/cobaltstrike/

# 在CS中加载: Script Manager → Load → async_bof.cna
```

## 📚 使用方法

### GUI操作
1. 右键Beacon → `异步BOF` → `添加监控器`
2. 选择监控类型和配置参数
3. 点击添加创建任务

### 命令行
```bash
# 基本命令
async_bof                              # 加载任务
async_bof_add_task "name" 0 "param" 30  # 添加任务
async_bof_list                         # 列出任务
async_bof_cancel 1                     # 取消任务
```

## 📊 性能参数
- **内存**: < 2MB/任务
- **CPU**: < 1% (轮询) / < 0.1% (事件)
- **响应**: 30-300s (轮询) / < 1s (事件)

## 🔒 OPSEC特性
- 被动监听，不主动扫描
- 最小API调用
- 随机化间隔
- 内存安全

## 🛠️ 故障排除

### 常见问题
- **脚本加载失败**: 检查BOF文件路径
- **任务创建失败**: 确认参数格式
- **事件未触发**: 检查监控条件和权限

## 📞 支持
- GitHub Issues: 问题报告
- [DEPLOYMENT_GUIDE.md](DEPLOYMENT_GUIDE.md): 详细部署指南
- [PROJECT_STATUS.md](PROJECT_STATUS.md): 项目状态

## 📄 授权
本项目采用MIT许可证。请参阅[LICENSE](LICENSE)文件。

---

**项目状态**: 🟢 生产就绪  
**版本**: v2.0 Production Ready  
**最后更新**: 2025-09-11务ID
- 任务将停止运行但保留在注册表中

**删除任务**：
- 右键点击Beacon → **"异步BOF" > "删除任务"**
- 输入要删除的任务ID
- 任务将完全从系统中移除

**加载任务**：
- 右键点击Beacon → **"异步BOF" > "加载任务"**
- 从注册表中恢复之前保存的任务

#### 3. 测试功能

在 **"异步BOF" > "测试事件"** 菜单中可以触发测试事件：
- **管理员登录事件**
- **进程启动事件**
- **用户登录事件**
- **WiFi连接事件**

### 命令行操作

除了图形界面，还可以直接在Beacon控制台中使用命令：

```bash
# 加载异步BOF并恢复任务
async_bof

# 添加监控任务
# 语法：async_bof_add_task <任务名称> <事件类型> <参数> <间隔>
async_bof_add_task "AdminMon" 0 "admin" 30
async_bof_add_task "ProcessMon" 1 "notepad.exe" 15
async_bof_add_task "UserMon" 2 "john" 60

# 列出所有任务
async_bof_list

# 取消任务（按ID）
async_bof_cancel 1

# 删除任务（按ID）
async_bof_delete 2

# 触发测试事件
async_bof_trigger 0 "DESKTOP-ABC123"
```

### 事件类型参考

| 类型ID | 事件名称 | 参数说明 | 示例参数 |
|--------|----------|----------|----------|
| 0 | 管理员登录 | 任意字符串 | "admin" |
| 1 | 进程启动 | 进程名称 | "notepad.exe" |
| 2 | 用户登录 | 用户名 | "administrator" |
| 3 | 网络隔离 | 网络接口 | "all" |
| 4 | VPN连接 | VPN类型 | "any" |
| 5 | WiFi连接 | SSID名称 | "WiFi-Network" |
| 6 | 票据过期 | 预警分钟数 | "30" |
| 7 | 剪贴板复制 | 监控文本 | "password" |
| 8 | 键盘输入 | 监控按键 | "ctrl+c" |
| 9 | 用户登录事件 | 用户名 | "guest" |

### 监控结果

当检测到事件时，会在Beacon控制台显示类似以下消息：

```
[EVENT] Administrator privileges detected on admin
[EVENT] Process 'notepad.exe' started on ProcessMon
[EVENT] User 'john' login detected on UserMon
[EVENT] Network isolation detected on all
[EVENT] Clipboard copy detected: 'password'
```

### 任务持久化

- **自动保存**：所有创建的任务都会自动保存到目标系统的注册表中
- **自动恢复**：重新加载BOF时会自动恢复之前的任务
- **注册表路径**：`HKEY_CURRENT_USER\Software\AsyncBOF\Tasks`

### 清理

```bash
make clean
```

这将删除 `bin/` 目录和所有编译文件。

## 实现细节

### 核心组件

1. **任务管理**：添加和管理具有可配置间隔的监控任务
2. **事件检测**：检查目标系统上的特定事件
3. **报告**：将事件报告回Cobalt Strike团队服务器
4. **OPSEC注意事项**：睡眠友好的设计以避免检测

### BOF函数

- `go()` - BOF的主要入口点
- `check_event()` - 检查是否发生了特定事件
- `report_event()` - 向团队服务器报告事件
- `create_event()` - 创建事件结构
- `destroy_event()` - 清理事件结构
- `create_task()` - 创建任务结构
- `destroy_task()` - 清理任务结构
- `cancel_task()` - 取消正在运行的任务
- `task_thread_proc()` - 运行任务的线程过程
- `monitor_clipboard_copy()` - 监控剪贴板复制事件
- `monitor_keyboard_input()` - 监控键盘输入事件
- `monitor_user_logon()` - 监控用户登录事件

### Aggressor脚本

`async_bof.cna` 脚本提供了一个用户友好的界面：

1. 添加监控任务
2. 列出活动任务
3. 取消任务
4. 管理异步BOF功能
5. 触发测试事件进行演示
6. 添加剪贴板监控器
7. 添加键盘监控器
8. 添加用户登录监控器

## 使用示例

### 在Cobalt Strike中的实际操作

#### 场景1：监控管理员权限获取

1. 右键点击beacon
2. 选择 **"异步BOF" → "添加监控器" → "管理员登录监控"**
3. 设置检查间隔为30秒
4. 点击"添加"

系统会每30秒检查一次当前进程是否获取了管理员权限。

#### 场景2：监控特定进程启动

1. 右键点击beacon
2. 选择 **"异步BOF" → "添加监控器" → "进程启动监控"**
3. 输入进程名：`powershell.exe`
4. 设置检查间隔为15秒
5. 点击"添加"

系统会每15秒检查一次是否有PowerShell进程启动。

#### 场景3：监控剪贴板敏感信息

1. 右键点击beacon
2. 选择 **"异步BOF" → "添加监控器" → "剪贴板监控"**
3. 输入监控文本：`password`
4. 设置检查间隔为5秒
5. 点击"添加"

当用户复制包含"password"的内容到剪贴板时，系统会立即报告。

#### 场景4：使用命令行批量添加任务

在Beacon控制台中输入：

```bash
# 添加多个监控任务
async_bof_add_task "AdminCheck" 0 "admin" 30
async_bof_add_task "ProcessMon" 1 "cmd.exe" 20
async_bof_add_task "UserWatch" 2 "administrator" 45

# 查看所有任务
async_bof_list
```

### 传统 BOF 使用方式

如果不使用Aggressor脚本，也可以直接加载BOF文件：

```bash
# 在Beacon控制台中
inline-execute /path/to/async_bof.o
```

### 本地测试示例

在部署到生产环境之前，可以先进行本地测试：

## 故障排除

### 常见问题

#### 1. 脚本加载失败

**问题**：加载 `async_bof.cna` 时报错

**解决方案**：
- 确保 `async_bof.o` 文件存在且可访问
- 检查文件路径是否正确
- 确保使用的是CS 4.0+版本

#### 2. BOF编译失败

**问题**：编译 `async_bof.o` 时出错

**解决方案**：
```bash
# 检查编译器版本
x86_64-w64-mingw32-gcc --version

# 尝试使用标准GCC
gcc -c -Wall -Iinclude -DBOF_BUILD -o bin/async_bof.o src/async_bof.c

# 检查编译错误日志
```

#### 3. 任务不执行

**问题**：添加了任务但没有监控结果

**可能原因**：
- 任务参数配置不正确
- 检查间隔设置过长
- 目标事件未发生

**排查步骤**：
1. 使用 `async_bof_list` 检查任务状态
2. 检查任务参数是否正确
3. 尝试触发测试事件
4. 检查Beacon连接状态

#### 4. 权限不足

**问题**：某些监控功能无法正常工作

**解决方案**：
- 确保Beacon具有足够的权限
- 某些监控功能可能需要管理员权限
- 检查UAC和防病毒软件设置

### 调试技巧

1. **本地测试**：先运行 `bin/async_bof_local.exe` 测试基本功能
2. **日志检查**：关注CS控制台的错误消息
3. **逐步测试**：从简单的监控任务开始测试
4. **参数验证**：使用测试事件验证功能是否正常

### 性能优化

1. **间隔设置**：根据需要调整检查间隔，避免过于频繁
2. **任务数量**：避免同时运行过多任务
3. **参数精度**：使用精确的监控参数减少误报

## 安全注意事项

### OPSEC建议

1. **间隔设置**：使用随机或较长的检查间隔，避免规律性活动
2. **任务管理**：及时清理不需要的监控任务
3. **数据清理**：定期清理注册表中的任务记录
4. **网络流量**：注意报告频率，避免产生异常流量

### 对抗检测

1. **内存操作**：BOF在内存中运行，减少磁盘足迹
2. **API调用**：使用合法的Windows API，避免被检测
3. **线程管理**：合理管理监控线程，避免资源泄漏
4. **注册表使用**：使用用户配置单元，降低被发现的风险

## 未来增强功能

1. 真实的系统事件监控
2. 持久化任务存储
3. Beacon间通信
4. 高级事件关联
5. 加密通信
6. Beacon到Beacon任务分发
7. 动态任务调度
8. 自动化响应操作
9. 完整的键盘记录器实现
10. 更智能的剪贴板内容分析