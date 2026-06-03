#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <chrono>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace ftxui;

static constexpr int GRID_W = 40;
static constexpr int GRID_H = 20;
static constexpr int TARGET_FPS = 15;

struct Point {
    int x, y;
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};

enum SnakeDir { SD_UP, SD_DOWN, SD_LEFT, SD_RIGHT };

class SnakeGame {
public:
    SnakeDir direction     = SD_RIGHT;
    SnakeDir nextDirection = SD_RIGHT;
    std::vector<Point> body;
    Point food;
    bool gameOver  = false;
    bool paused    = false;
    int  score     = 0;
    int  highScore = 0;
    int  moveTimer = 0;
    int  speed     = 1;

    SnakeGame() { Reset(); }

    void Reset() {
        body.clear();
        body.push_back({GRID_W / 2, GRID_H / 2});
        body.push_back({GRID_W / 2 - 1, GRID_H / 2});
        body.push_back({GRID_W / 2 - 2, GRID_H / 2});
        direction     = SD_RIGHT;
        nextDirection = SD_RIGHT;
        gameOver      = false;
        paused        = false;
        score         = 0;
        moveTimer     = 0;
        speed         = 1;
        SpawnFood();
    }

    void SpawnFood() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(0, GRID_W - 1);
        std::uniform_int_distribution<int> distY(0, GRID_H - 1);

        do {
            food.x = distX(gen);
            food.y = distY(gen);
        } while (IsSnake(food));
    }

    bool IsSnake(Point p) const {
        for (const auto& s : body) {
            if (s == p) return true;
        }
        return false;
    }

    void SetDirection(SnakeDir d) {
        if ((direction == SD_UP    && d == SD_DOWN)  ||
            (direction == SD_DOWN  && d == SD_UP)    ||
            (direction == SD_LEFT  && d == SD_RIGHT) ||
            (direction == SD_RIGHT && d == SD_LEFT)) {
            return;
        }
        nextDirection = d;
    }

    void Update() {
        if (gameOver || paused) return;

        ++moveTimer;
        if (moveTimer < (6 - speed)) return;
        moveTimer = 0;

        direction = nextDirection;

        Point head = body.front();
        switch (direction) {
        case SD_UP:    head.y--; break;
        case SD_DOWN:  head.y++; break;
        case SD_LEFT:  head.x--; break;
        case SD_RIGHT: head.x++; break;
        }

        if (head.x < 0 || head.x >= GRID_W ||
            head.y < 0 || head.y >= GRID_H) {
            gameOver = true;
            if (score > highScore) highScore = score;
            return;
        }

        for (const auto& s : body) {
            if (s == head) {
                gameOver = true;
                if (score > highScore) highScore = score;
                return;
            }
        }

        body.insert(body.begin(), head);

        if (head == food) {
            score += 10;
            if (score % 50 == 0 && speed < 5) ++speed;
            SpawnFood();
        } else {
            body.pop_back();
        }
    }
};

int main() {
    auto screen = ScreenInteractive::Fullscreen();
    SnakeGame game;

    auto renderer = Renderer([&] {
        if (game.gameOver) {
            return vbox({
                       text("  GAME  OVER  ") | bold | center |
                           color(Color::RedLight),
                       separator(),
                       text("  Score: " + std::to_string(game.score)) | center,
                       text("  High Score: " + std::to_string(game.highScore)) |
                           center | color(Color::YellowLight),
                       separator(),
                       text("  Press R to Restart") | center | dim,
                       text("  Press ESC to Quit ") | center | dim,
                   }) |
                   border | center;
        }

        if (game.paused) {
            return vbox({
                       text("  PAUSED  ") | bold | center |
                           color(Color::YellowLight),
                       separator(),
                       text("  Press P to Resume") | center | dim,
                       text("  Press ESC to Quit ") | center | dim,
                   }) |
                   border | center;
        }

        Elements rows;
        for (int y = 0; y < GRID_H; ++y) {
            Elements cells;
            for (int x = 0; x < GRID_W; ++x) {
                Point p{x, y};
                Color cellColor = Color::Default;

                if (p == game.body.front()) {
                    cellColor = Color::GreenLight;
                } else if (game.IsSnake(p)) {
                    cellColor = Color::Green;
                } else if (p == game.food) {
                    cellColor = Color::RedLight;
                }

                cells.push_back(text(" ") | bgcolor(cellColor));
            }
            rows.push_back(hbox(std::move(cells)));
        }

标准string标题 ="  SNAKE   Score: " +
                            std::to_string(game.score) +
                            " 高分: "+ std::to_string(game.highScore) +
                            " 速度: "+ std::to_string(game.speed);
        返回 方框({
            文本(标题)| 粗体 | 居中,
            vbox(std::move(rows))| 边框,
            text(" 箭头：移动 P：暂停 ESC：退出")| 居中 | 亮度,
        });
    });

    自动组件 = 渲染器;
组件 |=CatchEvent([&](事件e) -> 布尔值 {
        如果 (e == Event::ArrowUp)    {game.SetDirection(SD_UP);    返回 真; }
        如果 (e == Event::ArrowDown)  {game.设置方向(SD_DOWN);  返回 真; }
        如果 (e == 事件::方向键左)  {游戏.设置方向(SD_LEFT);  返回 真; }
        如果 (e == 事件::方向键右) {游戏.设置方向(SD_RIGHT); 返回 真; }

        如果 (e.是字符()) {
            std::string ch = e.character();
            if (ch == "w" || ch == "W") { game.SetDirection(SD_UP);    return true; }
            如果 (ch =="s"|| ch =="S") {game.SetDirection(SD_DOWN);  返回 真; }
            如果 || ch == game.SetDirectionSD_LEFT)
            如果 || ch == {game.SetDirection(SD_RIGHT); 返回 真; 
 {游戏。暂停= !游戏。暂停; 返回 真;
            如果 ch ==|| ch ==|| ch =="n"|| ch =="N") {
                如果 (game.gameOver) {game.重置(); 返回 真; }
            }
        }

        如果 ({
屏幕。退出();
            返回 真;
        }

        返回 假;
    });

    std::atomic<bool> running = true;
    循环loop(&screen, component);

    while (running && !loop.HasQuitted()) {
游戏。更新();
        screen.RequestAnimationFrame();
循环。运行一次();
std::this_thread::休眠(
            std::chrono::milliseconds(1000 / TARGET_FPS));
    }

    返回 0;
}
