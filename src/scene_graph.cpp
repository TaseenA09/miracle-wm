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

#include "scene_graph.h"
#include <algorithm>

using namespace miracle;

namespace
{
constexpr SceneNodeId NULL_PARENT_ID = UINT16_MAX;
}

SceneGraph::SceneGraph() :
    root_(this, next_id_++, NULL_PARENT_ID)
{
    nodes_.reserve(64);
}

SceneNode& SceneGraph::root()
{
    return root_;
}

SceneNode* SceneGraph::create(SceneNodeId parent)
{
    nodes_.push_back(SceneNode(this, next_id_++, parent));
    return &nodes_.back();
}

SceneNode::SceneNode(SceneGraph* graph, SceneNodeId id, SceneNodeId parent_id)
        : graph_ { graph },
          id { id },
          parent_id { parent_id }
        {
        }


SceneNode& SceneNode::operator=(const SceneNode& other)
{
    graph_ = other.graph_;
    parent_id = other.parent_id;
    children = other.children;
    size_ = other.size_;
    transform_ = other.transform_;
    opacity_ = other.opacity_;
    return *this;
}

SceneNode* SceneNode::add_child()
{
    auto node = graph_->create(id);
    children.push_back(node->id);
    return node;
}

void SceneNode::remove_child(SceneNodeId id)
{
    auto const initial_size = children.size();
    children.erase(std::ranges::find(children, id));
    if (initial_size == children.size())
        return;

    std::erase_if(graph_->nodes_, [id](auto const node)
    {
        return node.id == id;
    });
}

SceneNode* SceneNode::parent() const
{
    if (parent_id == NULL_PARENT_ID)
        return nullptr;

    auto const& nodes = graph_->nodes_;
    auto const it = std::ranges::find_if(nodes
        ,
        [this](auto const& node)
        {
            return node.id == parent_id;
        });

    if (it == nodes.end())
        return nullptr;

    return const_cast<SceneNode*>(&(*it));
}

glm::vec2 SceneNode::position() const
{
    return position_;
}

void SceneNode::position(glm::vec2 const& position)
{
    position_ = position;
}

glm::vec2 SceneNode::size() const
{
    return size_;
}

void SceneNode::size(glm::vec2 const& size)
{
    size_ = size;
}

glm::mat4 SceneNode::transform() const
{
    return transform_;
}

void SceneNode::transform(glm::mat4 const& transform)
{
    transform_ = transform;
}

float SceneNode::opacity() const
{
    return opacity_;
}

void SceneNode::opacity(float opacity)
{
    opacity_ = opacity;
}
