#贪吃蛇游戏（FTXUI）

基于 FTXUI Component API 的贪吃蛇游戏，参照官方 [examples/component](https://github.com/ArthurSonzogni/FTXUI/tree/main/examples/component) 编写。

## 特性

- 使用 `ScreenInteractive` + `Loop` + `Renderer` + `CatchEvent` 标准 FTXUI 组件模式
- 贪吃蛇自动移动，吃食物增长，撞墙/撞自己游戏结束
- 加速机制：每 50 分提速一档（共 5 档）
- 暂停/继续、最高分记录

## 编译

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
.\snake_game.exe
```

FTXUI 通过 FetchContent 自动下载。

## 操作

| 按键 | 功能 |
|------|------|
| ↑↓←→ / WASD | 控制方向 |
| P | 暂停/继续 |
