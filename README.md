# 异步BOF (Beacon Object Files) 框架

异步Beacon Object Files概念的实现。它提供了一个框架，用于运行可以检测事件并报告回Cobalt Strike团队服务器的异步监控任务。

## 概念

传统的BOF是同步的，这限制了它们的潜力。该框架支持：

1. 在后台运行监控任务
2. 基于事件的触发
3. 睡眠友好的操作
4. OPSEC安全执行

## 特性

- 异步任务执行
- 多种事件监控功能
- 睡眠友好设计
- Cobalt Strike集成
- 操作员友好界面
- 任务持久化
- 任务取消
- 真实事件监控

## 结构

```
include/              # 头文件
src/                  # C源文件
async_bof.cna         # Cobalt Strike aggressor脚本
Makefile              # 构建脚本
README.md             # 本文档
```

## 支持的事件类型

1. 管理员登录
2. 进程启动
3. 用户登录
4. 网络隔离
5. VPN连接
6. WiFi连接
7. 票据过期
8. 剪贴板复制监控
9. 键盘输入监控
10. 用户登录监控
11. 自定义事件

## 入门指南

### 先决条件

- 用于Windows交叉编译的mingw-w64
- Cobalt Strike 4.0+

## 构建和部署

### 先决条件

- 用于Windows交叉编译的mingw-w64（可选，用于BOF编译）
- 标准GCC编译器（用于本地测试）
- Cobalt Strike 4.0+

### 构建

#### BOF编译（用于Cobalt Strike）

```bash
# 如果有mingw-w64编译器
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -o bin/async_bof.o src/async_bof.c

# 或者使用Makefile（如果系统支持make）
make
```

#### 本地测试编译

```bash
# 编译为目标文件
gcc -c -Wall -Iinclude -DLOCAL_TEST -o bin/async_bof_local.o src/async_bof.c

# 编译为可执行文件
gcc -Wall -Iinclude -DLOCAL_TEST -o bin/async_bof_local.exe src/async_bof.c

# 或者使用Makefile
make local
```

#### 运行本地测试

```bash
# Windows
bin\async_bof_local.exe

# Linux/macOS
./bin/async_bof_local
```

这将测试基本的任务创建、事件监控和报告功能。

### 部署到Cobalt Strike

#### 步骤1：编译BOF文件

```bash
# 使用mingw-w64编译器编译BOF
x86_64-w64-mingw32-gcc -c -Wall -Iinclude -DBOF_BUILD -o bin/async_bof.o src/async_bof.c

# 或者使用标准GCC（如果在Linux环境）
gcc -c -Wall -Iinclude -DBOF_BUILD -o bin/async_bof.o src/async_bof.c
```

#### 步骤2：加载Aggressor脚本

1. 打开Cobalt Strike客户端
2. 在菜单栏选择 **"Cobalt Strike" > "Script Manager"**
3. 点击 **"Load"** 按钮
4. 选择项目中的 `async_bof.cna` 文件
5. 点击 **"打开"** 加载脚本

脚本加载成功后，控制台会显示：
```
[*] 异步BOF Aggressor脚本加载成功
[*] 事件类型:
    0 - 管理员登录
    1 - 进程启动
    2 - 用户登录
    3 - 网络隔离
    4 - VPN连接
    5 - WiFi连接
    6 - 票据过期
    7 - 剪贴板复制
    8 - 键盘输入
    9 - 用户登录事件
[*] 使用右键菜单 '异步BOF' 进行图形界面操作
```

#### 步骤3：复制BOF文件到CS目录（可选）

```bash
# 将编译好的BOF文件复制到Cobalt Strike目录
cp bin/async_bof.o /path/to/cobaltstrike/

# 或者在Windows中
copy bin\async_bof.o C:\path\to\cobaltstrike\
```

**注意**：如果不复制到CS目录，需要确保 `async_bof.o` 文件与 `async_bof.cna` 在同一目录下。

## 在Cobalt Strike中使用

### 图形界面操作

#### 1. 添加监控任务

1. 在Beacon列表中 **右键点击** 目标Beacon
2. 选择菜单 **"异步BOF" > "添加监控器"**
3. 选择要添加的监控类型：
   - **管理员登录监控** - 检测管理员权限获取
   - **进程启动监控** - 监控特定进程启动
   - **用户登录监控** - 监控特定用户登录
   - **网络隔离监控** - 检测网络连接断开
   - **剪贴板监控** - 监控剪贴板内容变化
   - **键盘监控** - 检测键盘输入活动
   - **用户登录事件监控** - 监控用户登录事件

4. 在弹出的对话框中配置参数：
   - **监控参数**：根据监控类型填写（如进程名、用户名等）
   - **检查间隔**：设置检查频率（秒）

5. 点击 **"添加"** 按钮创建监控任务

#### 2. 管理现有任务

**列出所有任务**：
- 右键点击Beacon → **"异步BOF" > "列出任务"**
- 查看当前所有活动的监控任务

**取消任务**：
- 右键点击Beacon → **"异步BOF" > "取消任务"**
- 输入要取消的任务ID
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