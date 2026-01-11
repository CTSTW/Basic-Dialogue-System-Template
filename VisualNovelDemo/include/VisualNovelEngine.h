#pragma once
#ifndef VISUAL_NOVEL_ENGINE_H
#define VISUAL_NOVEL_ENGINE_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include "DialogueSystem.h"
#include "CharacterRenderer.h"
#include "ScriptInterpreter.h"

namespace VisualNovel {
    
    // 游戏状态
    enum class GameState {
        TITLE_SCREEN,
        DIALOGUE,
        CHOICE,
        MENU,
        GAME_OVER,
        ENDING
    };
    
    // 存档数据结构
    struct SaveData {
        std::string saveName;
        std::string timestamp;
        std::string currentScript;
        int scriptPosition;
        std::map<std::string, int> characterAffection;
        std::map<std::string, bool> flags;
        std::string backgroundImage;
        std::vector<std::string> activeCharacters;
    };
    
    // 游戏设置
    struct GameSettings {
        float textSpeed = 50.0f;  // 文字显示速度（字/秒）
        float autoPlayDelay = 3.0f;  // 自动播放延迟（秒）
        bool skipReadText = true;    // 跳过已读文本
        bool voiceEnabled = true;    // 语音启用
        float bgmVolume = 0.8f;      // BGM音量
        float seVolume = 0.7f;       // 音效音量
        float voiceVolume = 1.0f;    // 语音音量
    };
    
    // 视觉小说引擎核心类
    class VisualNovelEngine {
    private:
        GameState currentState;
        GameSettings settings;
        std::unique_ptr<DialogueSystem> dialogueSystem;
        std::unique_ptr<CharacterRenderer> characterRenderer;
        std::unique_ptr<ScriptInterpreter> scriptInterpreter;
        
        std::string currentScript;
        std::vector<SaveData> saveSlots;
        std::map<std::string, std::string> variables;
        std::map<std::string, bool> flags;
        
        // 回调函数
        std::function<void(const std::string&)> onTextDisplay;
        std::function<void(const std::string&)> onBackgroundChange;
        std::function<void(const std::string&)> onBgmPlay;
        std::function<void()> onChoicePresent;
        
    public:
        VisualNovelEngine();
        ~VisualNovelEngine();
        
        // 初始化
        bool Initialize(const std::string& configPath);
        
        // 游戏流程控制
        void StartGame(const std::string& scriptPath);
        void LoadGame(int slot);
        void SaveGame(int slot, const std::string& saveName);
        void QuickSave();
        void QuickLoad();
        
        // 更新逻辑
        void Update(float deltaTime);
        void ProcessInput(int key);
        void ProcessMouseClick(int x, int y);
        
        // 脚本控制
        void NextLine();
        void SkipDialogue();
        void AutoPlay(bool enabled);
        void ShowHistory();
        void ShowMenu();
        
        // 回调设置
        void SetTextCallback(std::function<void(const std::string&)> callback);
        void SetBackgroundCallback(std::function<void(const std::string&)> callback);
        void SetBgmCallback(std::function<void(const std::string&)> callback);
        void SetChoiceCallback(std::function<void()> callback);
        
        // 状态获取
        GameState GetCurrentState() const;
        const GameSettings& GetSettings() const;
        void UpdateSettings(const GameSettings& newSettings);
        
        // 变量操作
        void SetVariable(const std::string& name, const std::string& value);
        std::string GetVariable(const std::string& name) const;
        void SetFlag(const std::string& flag, bool value);
        bool GetFlag(const std::string& flag) const;
        
    private:
        void LoadScript(const std::string& path);
        void ProcessScriptCommand(const std::string& command);
        void UpdateCharacters(float deltaTime);
        void HandleChoiceSelection(int choiceIndex);
        void PlayVoice(const std::string& voiceFile);
        void PlaySoundEffect(const std::string& seFile);
    };
    
} // namespace VisualNovel

#endif // VISUAL_NOVEL_ENGINE_H
