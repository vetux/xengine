/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_WINDOWATTRIBUTES_HPP
#define MANA_WINDOWATTRIBUTES_HPP

namespace engine {
    /**
     * The attributes of a window which are required for creation.
     * Some attributes may be changed for an existing window, for which
     * individual member functions are added to the window class MANA_EXPORT.
     */
    struct MANA_EXPORT WindowAttributes {
        bool resizable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoMinimize = true;
        bool alwaysOnTop = false;
        bool maximized = false;
        bool centerCursor = true;
        bool transparentFramebuffer = false;
        bool focusOnShow = true;
        bool scaleToMonitor = false;

        int bitsRed = 8;
        int bitsGreen = 8;
        int bitsBlue = 8;
        int bitsAlpha = 8;
        int bitsDepth = 24;
        int bitsStencil = 8;

        int msaaSamples = 4;

        bool sRGBCapable = false;

        bool doubleBuffer = true;

        int swapInterval = 0;
    };
}

#endif //MANA_WINDOWATTRIBUTES_HPP
