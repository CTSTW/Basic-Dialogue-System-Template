#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>
#include "VisualNovelEngine.h"

using namespace VisualNovel;

// 简单的控制台界面模拟
class ConsoleInterface {
private:
    VisualNovelEngine* engine;
    bool running;
    
public:
    ConsoleInterface(VisualNovelEngine* eng) : engine(eng), running(true) {}
    
    void Run() {
        std::cout << "=== 视觉小说引擎控制台 ===" << std::endl;
        std::cout << "命令: next, skip, menu, save, load, quit" << std::endl;
        
        while (running) {
            std::cout << "\n> ";
            std::string command;
            std::getline(std::cin, command);
            
            ProcessCommand(command);
        }
    }
    
    void ProcessCommand(const std::string& cmd) {
        if (cmd == "next" || cmd == "n") {
            engine->NextLine();
        } else if (cmd == "skip" || cmd == "s") {
            engine->SkipDialogue();
        } else if (cmd == "menu" || cmd == "m") {
            engine->ShowMenu();
        } else if (cmd == "save") {
            engine->SaveGame(0, "手动存档");
            std::cout << "游戏已保存" << std::endl;
        } else if (cmd == "load") {
            engine->LoadGame(0);
            std::cout << "游戏已加载" << std::endl;
        } else if (cmd == "quit" || cmd == "q") {
            running = false;
            std::cout << "退出游戏..." << std::endl;
        } else if (cmd == "auto") {
            engine->AutoPlay(true);
            std::cout << "启用自动播放" << std::endl;
        } else if (cmd == "history" || cmd == "h") {
            engine->ShowHistory();
        } else {
            std::cout << "未知命令" << std::endl;
        }
    }
};

// 文本显示回调示例
void OnTextDisplay(const std::string& text) {
    std::cout << "\n--- 对话 ---" << std::endl;
    std::cout << text << std::endl;
    std::cout << "-------------" << std::endl;
}

// 背景切换回调示例
void OnBackgroundChange(const std::string& bgName) {
    std::cout << "[背景切换: " << bgName << "]" << std::endl;
}

// BGM播放回调示例
void OnBgmPlay(const std::string& bgmName) {
    std::cout << "[播放BGM: " << bgmName << "]" << std::endl;
}

// 选择支显示回调示例
void OnChoicePresent() {
    std::cout << "\n*** 请做出选择 ***" << std::endl;
}

int main() {
    std::cout << "启动视觉小说引擎..." << std::endl;
    
    // 创建引擎实例
    auto engine = std::make_unique<VisualNovelEngine>();
    
    // 设置回调
    engine->SetTextCallback(OnTextDisplay);
    engine->SetBackgroundCallback(OnBackgroundChange);
    engine->SetBgmCallback(OnBgmPlay);
    engine->SetChoiceCallback(OnChoicePresent);
    
    // 初始化引擎
    if (!engine->Initialize("config/game_config.ini")) {
        std::cerr << "引擎初始化失败!" << std::endl;
        return 1;
    }
    
    std::cout << "引擎初始化成功!" << std::endl;
    
    // 启动游戏
    std::cout << "开始游戏..." << std::endl;
    engine->StartGame("data/scripts/prologue.txt");
    
    // 创建控制台界面
    ConsoleInterface console(engine.get());
    
    // 在主线程中运行引擎更新
    std::thread engineThread([&engine]() {
        auto lastTime = std::chrono::high_resolution_clock::now();
        
        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            engine->Update(deltaTime);
            
            // 控制更新频率
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
        }
    });
    
    // 运行控制台
    console.Run();
    
    // 清理
    engineThread.detach();
    
    std::cout << "游戏结束，感谢游玩!" << std::endl;
    return 0;
}
