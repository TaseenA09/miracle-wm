/**
Copyright (C) 2024  Matthew Kosarek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef MIRACLEWM_SCENE_GRAPH_H
#define MIRACLEWM_SCENE_GRAPH_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace miracle
{
typedef uint16_t SceneNodeId;

class SceneGraph;

class SceneNode
{
public:
    SceneNode& operator=(const SceneNode& other);

    /// Add a child to the node.
    ///
    /// Users may store the pointer to this child but they must
    /// be certain that they will stop using the pointer once
    /// the child has been removed.
    ///
    /// \returns the added node
    SceneNode* add_child();

    /// Remove the child provided by the \p id.
    ///
    /// If the \p id is invalid, nothing happens.
    ///
    /// \param id of the node
    void remove_child(SceneNodeId id);

    glm::vec2 position() const;
    void position(glm::vec2 const& position);
    glm::vec2 size() const;
    void size(glm::vec2 const& size);
    glm::mat4 transform() const;
    void transform(glm::mat4 const& transform);
    float opacity() const;
    void opacity(float opacity);

    /// The id of this node.
    SceneNodeId const id;

private:
    friend SceneGraph;

    SceneNode(SceneGraph* graph, SceneNodeId id, SceneNodeId parent_id);

    SceneGraph* graph_;

    /// The unique id of this node's parent.
    SceneNodeId parent_id;

    /// The children of this node.
    std::vector<SceneNodeId> children;

    /// The position of the node relative to the #parent.
    glm::vec2 position_ = glm::vec2(0.f);

    /// The size of the node.
    ///
    /// This size may extend beyond the size of its parent, although
    /// it will be clipped by the scene.
    glm::vec2 size_ = glm::vec2(1.f);
    glm::mat4 transform_ = glm::mat4(1.f);
    float opacity_ = 1.0f;
};

/// The scene graph.
///
/// The scene graph manages the list of nodes in the scene.
/// It is a lightweight data structure providing ways to create,
/// find, and remove nodes.
///
/// When a node changes, the scene graph will be republished for
/// rendering in a cache-friendly form.
///
/// This interface purposefully avoids smart pointers for the sake of
/// rendering efficiency.
class SceneGraph
{
public:
    SceneGraph();

    SceneNode& root();

private:
    friend SceneNode;
    SceneNode* create(SceneNodeId parent);
    std::vector<SceneNode> nodes_;
    SceneNodeId next_id_ = 0;
    SceneNode root_;
};
}

#endif