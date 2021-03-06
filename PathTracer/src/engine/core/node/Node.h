#ifndef NODE_H
#define NODE_H

#include "Includes.hpp"

#include "engine/core/model/transform.h"
#include "engine/common/rendering/Renderer.h"

namespace Prehistoric
{
    class CoreEngine;

    class Node
    {
    public:
        Node() : parent(nullptr), enabled(true) {}
        virtual ~Node() {}

        virtual void PreUpdate(CoreEngine* engine);
        virtual void PreRender(Renderer* renderer);

        Node* AddChild(const std::string& key, Node* child);

        inline const Transform& getWorldTransform() const { return worldTransform; }
        inline Transform& getWorldTransform() { return worldTransform; }

        inline Node* getChild(const std::string& key) const { return children.at(key).get(); }
        void deleteChild(Node* node);

        inline void Move(const Vector3f& d) { worldTransform.setPosition(worldTransform.getPosition() + d); }
        inline void Rotate(const Vector3f& d) { worldTransform.setRotation(worldTransform.getRotation() + d); }
        inline void Scale(const Vector3f& d) { worldTransform.setScaling(worldTransform.getScaling() + d); }

        inline void SetPosition(const Vector3f& d) { worldTransform.setPosition(d); }
        inline void SetRotation(const Vector3f& d) { worldTransform.setRotation(d); }
        inline void SetScale(const Vector3f& d) { worldTransform.setScaling(d); }

        inline bool isEnabled() const { return enabled; }
        inline void setEnabled(bool enabled) { this->enabled = enabled; }

        std::unordered_map<std::string, Node*> getChildren() const;

        Node(const Node&) = default;
    protected:
        std::unordered_map<std::string, std::unique_ptr<Node>> children;
        Node* parent;

        Transform worldTransform;
        bool enabled;
    };
};

#endif