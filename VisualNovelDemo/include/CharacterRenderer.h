#pragma once
#ifndef CHARACTER_RENDERER_H
#define CHARACTER_RENDERER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <glm/glm.hpp>

namespace VisualNovel {
    
    // 角色精灵状态
    enum class CharacterState {
        IDLE,
        TALKING,
        EMOTION,
        ENTERING,
        EXITING,
        MOVING
    };
    
    // 角色表情
    struct CharacterExpression {
        std::string name;
        std::string texturePath;
        glm::vec2 uvOffset;
        glm::vec2 uvSize;
        std::vector<std::string> morphTargets;  // 表情变形目标
        float transitionTime;
    };
    
    // 角色位置
    struct CharacterPosition {
        std::string name;
        glm::vec2 screenPosition;  // 归一化坐标 0-1
        float depth;
        float scale;
        std::string layer;  // foreground, middle, background
    };
    
    // 角色动画
    struct CharacterAnimation {
        std::string name;
        std::vector<std::string> frames;
        float frameRate;
        bool loop;
        std::function<void()> onComplete;
    };
    
    // 角色精灵
    class CharacterSprite {
    private:
        std::string characterId;
        std::string currentExpression;
        CharacterState currentState;
        CharacterPosition position;
        
        std::map<std::string, CharacterExpression> expressions;
        std::map<std::string, CharacterAnimation> animations;
        
        glm::vec2 currentPosition;
        glm::vec2 targetPosition;
        float moveSpeed;
        float currentScale;
        float targetScale;
        
        float animationTimer;
        int currentFrame;
        bool isAnimating;
        
    public:
        CharacterSprite(const std::string& id);
        ~CharacterSprite();
        
        // 位置控制
        void SetPosition(const glm::vec2& pos);
        void MoveTo(const glm::vec2& target, float speed = 1.0f);
        void SetDepth(float depth);
        void SetScale(float scale);
        void SetLayer(const std::string& layer);
        
        // 表情控制
        void AddExpression(const std::string& name, const CharacterExpression& expr);
        void SetExpression(const std::string& exprName);
        void TransitionExpression(const std::string& exprName, float duration);
        const std::string& GetCurrentExpression() const;
        
        // 动画控制
        void AddAnimation(const std::string& name, const CharacterAnimation& anim);
        void PlayAnimation(const std::string& animName);
        void StopAnimation();
        bool IsAnimating() const;
        
        // 状态控制
        void SetState(CharacterState state);
        CharacterState GetState() const;
        
        // 更新
        void Update(float deltaTime);
        
        // 渲染数据获取
        glm::vec2 GetRenderPosition() const;
        float GetRenderDepth() const;
        float GetRenderScale() const;
        std::string GetCurrentTexture() const;
        glm::vec2 GetUVOffset() const;
        glm::vec2 GetUVSize() const;
        
        // 属性获取
        const std::string& GetCharacterId() const;
        const CharacterPosition& GetPositionInfo() const;
        
    private:
        void UpdateMovement(float deltaTime);
        void UpdateAnimation(float deltaTime);
        void UpdateExpression(float deltaTime);
    };
    
    // 角色渲染管理器
    class CharacterRenderer {
    private:
        std::map<std::string, std::unique_ptr<CharacterSprite>> characters;
        std::vector<std::string> renderOrder;  // 渲染顺序
        
        // 位置预设
        std::map<std::string, CharacterPosition> positionPresets;
        
        // 图层管理
        std::map<std::string, std::vector<std::string>> layerGroups;
        
    public:
        CharacterRenderer();
        ~CharacterRenderer();
        
        // 角色管理
        CharacterSprite* CreateCharacter(const std::string& id, 
                                       const std::string& configPath);
        CharacterSprite* GetCharacter(const std::string& id);
        void RemoveCharacter(const std::string& id);
        void ClearAllCharacters();
        
        // 位置预设
        void AddPositionPreset(const std::string& name, const CharacterPosition& pos);
        void SetCharacterPosition(const std::string& charId, const std::string& presetName);
        
        // 图层管理
        void AddToLayer(const std::string& charId, const std::string& layer);
        void SetLayerOrder(const std::vector<std::string>& order);
        
        // 批量操作
        void ShowAllCharacters();
        void HideAllCharacters();
        void FadeInAll(float duration);
        void FadeOutAll(float duration);
        
        // 更新
        void Update(float deltaTime);
        
        // 获取渲染列表
        std::vector<CharacterSprite*> GetRenderList() const;
        
        // 场景控制
        void EnterScene(const std::string& sceneConfig);
        void ExitScene(const std::string& sceneConfig);
        
    private:
        void LoadCharacterConfig(const std::string& charId, 
                                const std::string& configPath);
        void UpdateRenderOrder();
    };
    
} // namespace VisualNovel

#endif // CHARACTER_RENDERER_H
