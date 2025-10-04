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

#include "overview_mode.h"
#include "compositor_state.h"
#include "container.h"

using namespace miracle;

OverviewMode::OverviewMode(std::shared_ptr<CompositorState> const& state) :
    state(state)
{
}

void OverviewMode::enter()
{
    for (auto const& container : state->containers())
    {
        if (auto const sh = container.lock())
        {
        }
    }
}

void OverviewMode::exit()
{
}

void OverviewMode::advise_new_window(miral::WindowInfo const& info)
{
}
