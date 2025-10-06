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

#define GLM_ENABLE_EXPERIMENTAL
#include "overview_mode.h"
#include "compositor_state.h"
#include "container.h"
#include "output_interface.h"

#include <glm/ext/matrix_transform.hpp>

using namespace miracle;

namespace
{
struct OutputTransformInfo
{
    OutputInterface const* output;
    float x = 0;
    float current_y = 0;
    float max_y = 0;
};

void apply_transforms(std::vector<std::weak_ptr<Container>> const& containers)
{
    std::vector<OutputTransformInfo> info_list;

    for (auto const& c : containers)
    {
        if (auto const sh = c.lock())
        {
            auto const output = sh->get_output();
            if (!output)
                continue;

            auto const window = sh->window();
            if (!window)
                continue;

            auto it = std::ranges::find_if(info_list, [&output](auto const& i) { return i.output == output.get(); });
            if (it == info_list.end())
            {
                info_list.push_back({ output.get(), 0 });
                it = info_list.end() - 1;
            }
            auto const raw_area = geom::Rectangle {
                window->top_left(),
                window->size()
            };
            auto const scale_factor = 0.5f;

            glm::vec2 const scaled_size(
                raw_area.size.width.as_value() * scale_factor,
                raw_area.size.height.as_value() * scale_factor
            );

            auto& info = *it;
            if (info.x > info.output->get_area().size.width.as_value())
            {
                info.x = 0;
                info.current_y += info.max_y;
                info.max_y = 0;
            }

            glm::vec2 const new_position(info.x, info.current_y);
            auto const displacement = new_position - glm::vec2(raw_area.top_left.x.as_value(), raw_area.top_left.y.as_value());

            glm::mat4 transform(1.f);
            transform = glm::scale(transform, glm::vec3(scale_factor, scale_factor, 1.f));
            transform = glm::translate(transform, glm::vec3(displacement, 0.f));

            sh->mode_transform(transform);

            info.max_y = std::max(info.max_y, 2 * scaled_size.y);
            info.x += scaled_size.x * 2 + 20;
        }
    }
}
}

OverviewMode::OverviewMode(std::shared_ptr<CompositorState> const& state) :
    state(state)
{
}

bool OverviewMode::enter()
{
    if (active)
        return false;

    apply_transforms(state->containers());
    active = true;
    return true;
}

bool OverviewMode::exit()
{
    if (!active)
        return false;

    for (auto const& container : state->containers())
    {
        if (auto const sh = container.lock())
        {
            sh->mode_transform(glm::mat4(1.f));
        }
    }

    active = false;
    return true;
}

void OverviewMode::advise_new_window(miral::WindowInfo const& info)
{
    apply_transforms(state->containers());
}
