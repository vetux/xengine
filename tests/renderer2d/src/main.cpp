/**
 *  This file is part of xEngine, a C++ game engine library.
 *  Copyright (C) 2022  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "xng/xng.hpp"

xng::ImageRGBA loadImage(const std::filesystem::path &filePath) {
    auto imageParser = xng::StbiParser();
    auto data = xng::readFile(filePath.string());
    return imageParser.read(data, filePath.extension(), nullptr).get<xng::ImageRGBA>();
}

int main(int argc, char *argv[]) {
    auto displayDriver = xng::DisplayDriver::load(xng::GLFW);
    auto gpuDriver = xng::GpuDriver::load(xng::OPENGL_4_6);
    auto shaderCompiler = xng::SPIRVCompiler::load(xng::SHADERC);
    auto shaderDecompiler = xng::SPIRVDecompiler::load(xng::SPIRV_CROSS);

    auto window = displayDriver->createWindow("opengl", "Renderer 2D Test", {640, 480}, {});
    auto &input = window->getInput();
    auto &target = window->getRenderTarget();

    auto renderDevice = gpuDriver->createRenderDevice();

    xng::Renderer2D ren(*renderDevice, *shaderCompiler, *shaderDecompiler);

    auto imageB = loadImage("assets/awesomeface.png");
    auto imageA = loadImage("assets/tux.png");

    auto texA = ren.createTexture(imageA);
    auto texB = ren.createTexture(imageB);

    auto frameLimiter = xng::FrameLimiter(60);

    while (!window->shouldClose()) {
        auto delta = frameLimiter.newFrame();

        ren.renderBegin(target);

        auto targetSize = target.getDescription().size.convert<float>();

        ren.draw(xng::Vec2f(0, 0), targetSize, xng::ColorRGBA::green());

        ren.draw(xng::Vec2f(0, targetSize.y), xng::Vec2f(targetSize.x, 0), xng::ColorRGBA::green());

        ren.draw(xng::Rectf({}, imageA.getSize().convert<float>()),
                 xng::Rectf({}, imageA.getSize().convert<float>()),
                 texA);

        ren.draw(xng::Rectf({}, imageA.getSize().convert<float>()),
                 xng::ColorRGBA::yellow(),
                 false);

        ren.draw(xng::Rectf({}, imageB.getSize().convert<float>()),
                 xng::Rectf({(float) imageA.getWidth(), 0}, imageB.getSize().convert<float>()),
                 texB);

        ren.draw(xng::Rectf({(float) imageA.getWidth(), 0}, imageB.getSize().convert<float>()),
                 xng::ColorRGBA::yellow(),
                 false);

        ren.renderPresent();

        window->swapBuffers();
        window->update();
    }

    ren.destroyTexture(texA);
    ren.destroyTexture(texB);

    return 0;
}