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

#ifndef MIRACLE_OVERVIEW_MODE_H
#define MIRACLE_OVERVIEW_MODE_H

#include <memory>
#include <miral/window_info.h>

namespace miracle
{
class CompositorState;

/// Overview mode is similar to that of GNOME and similar desktops
/// whereby the user will easily be able to see the windows
/// on the current workspace and other workspaces.
///
/// This mode is meant as a prototype of future modes to come and
/// even user-written modes in WebAssembly. Only one mode shall
/// be active a time.
///
/// TODO: Handle workspace change
/// TODO: Handle output disappearance
class OverviewMode
{
public:
    explicit OverviewMode(std::shared_ptr<CompositorState> const& state);

    /// Enter overview mode.
    ///
    /// This will trigger an animation that will transform active windows to
    /// some predetermined location.
    bool enter();

    /// Exit overview mode.
    ///
    /// This will return windows to their original location.
    bool exit();

    /// Notification that a window has been created.
    ///
    /// Modes may use this notification to adjust the current render of this
    /// window or others accordingly.
    ///
    /// \param info the window that has been added
    void advise_new_window(miral::WindowInfo const& info);

private:
    bool active { false };
    std::shared_ptr<CompositorState> state;
};
}

#endif