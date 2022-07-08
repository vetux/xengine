/**
 *  xEngine - C++ game engine library
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

#ifndef XENGINE_ENGINE_HPP
#define XENGINE_ENGINE_HPP

#include "xengine-export.hpp"
#include "xengine.hpp"
#include "script/script.hpp"
#include "ecs/entitycontainer.hpp"
#include "ecs/entityhandle.hpp"
#include "ecs/components.hpp"
#include "ecs/entity.hpp"
#include "ecs/componentcontainer.hpp"
#include "ecs/system.hpp"
#include "ecs/ecs.hpp"
#include "ecs/componentpool.hpp"
#include "ecs/systems/animationsystem.hpp"
#include "ecs/systems/physicssystem.hpp"
#include "ecs/systems/rendersystem.hpp"
#include "ecs/systems/monoscriptingsystem.hpp"
#include "ecs/systems/audiosystem.hpp"
#include "ecs/systems/eventsystem.hpp"
#include "ecs/components/monoscriptcomponent.hpp"
#include "ecs/components/meshrendercomponent.hpp"
#include "ecs/components/skyboxcomponent.hpp"
#include "ecs/components/cameracomponent.hpp"
#include "ecs/components/particlecomponent.hpp"
#include "ecs/components/monosynccomponent.hpp"
#include "ecs/components/spriteanimationcomponent.hpp"
#include "ecs/components/spritecameracomponent.hpp"
#include "ecs/components/spriterendercomponent.hpp"
#include "ecs/components/transformcomponent.hpp"
#include "ecs/components/lightcomponent.hpp"
#include "ecs/components/physics/collidercomponent.hpp"
#include "ecs/components/physics/rigidbodycomponent.hpp"
#include "ecs/components/gui/buttoncomponent.hpp"
#include "ecs/components/gui/textcomponent.hpp"
#include "ecs/components/gui/canvascomponent.hpp"
#include "ecs/components/audio/audiosourcecomponent.hpp"
#include "ecs/components/audio/audiolistenercomponent.hpp"
#include "display/monitor.hpp"
#include "display/displaydriver.hpp"
#include "display/windowlistener.hpp"
#include "display/windowattributes.hpp"
#include "display/videomode.hpp"
#include "display/window.hpp"
#include "compat/imguicompat.hpp"
#include "compat/qtrenderwidget.hpp"
#include "algo/refcounter.hpp"
#include "algo/hashcombine.hpp"
#include "algo/crc.hpp"
#include "algo/counter.hpp"
#include "physics/collider.hpp"
#include "physics/world.hpp"
#include "physics/joint.hpp"
#include "physics/physicsdriver.hpp"
#include "physics/rigidbody.hpp"
#include "cast/numeric_cast.hpp"
#include "math/vector2.hpp"
#include "math/quaternion.hpp"
#include "math/vector3.hpp"
#include "math/rotation.hpp"
#include "math/normalize.hpp"
#include "math/grid.hpp"
#include "math/matrixmath.hpp"
#include "math/transform.hpp"
#include "math/matrix.hpp"
#include "math/rectangle.hpp"
#include "math/vector4.hpp"
#include "dl/library.hpp"
#include "audio/audiosource.hpp"
#include "audio/audiobuffer.hpp"
#include "audio/audioformat.hpp"
#include "audio/audiolistener.hpp"
#include "audio/audiodriver.hpp"
#include "audio/audiocontext.hpp"
#include "audio/audiodevice.hpp"
#include "driver/driver.hpp"
#include "driver/drivermanager.hpp"
#include "io/pak.hpp"
#include "io/substreambuf.hpp"
#include "io/message.hpp"
#include "io/readfile.hpp"
#include "io/archive.hpp"
#include "io/pakbuilder.hpp"
#include "io/protocol.hpp"
#include "io/byte.hpp"
#include "io/protocol/jsonprotocol.hpp"
#include "io/archive/pakarchive.hpp"
#include "io/archive/directoryarchive.hpp"
#include "io/archive/memoryarchive.hpp"
#include "io/schema/ecsschema.hpp"
#include "io/schema/mathschema.hpp"
#include "resource/resourceimporter.hpp"
#include "resource/resourcebundle.hpp"
#include "resource/resourceregistry.hpp"
#include "resource/resourceexporter.hpp"
#include "resource/resourcehandle.hpp"
#include "resource/uri.hpp"
#include "resource/resource.hpp"
#include "crypto/aes.hpp"
#include "crypto/sha.hpp"
#include "crypto/random.hpp"
#include "app/application.hpp"
#include "input/inputlistener.hpp"
#include "input/input.hpp"
#include "input/device/keyboard.hpp"
#include "input/device/mouse.hpp"
#include "input/device/gamepad.hpp"
#include "input/device/buttonstate.hpp"
#include "compression/gzip.hpp"
#include "animation/sprite/spritekeyframe.hpp"
#include "animation/sprite/spriteanimation.hpp"
#include "render/shaderinclude.hpp"
#include "render/scenerenderer.hpp"
#include "render/2d/renderer2d.hpp"
#include "render/forward/forwardrenderer.hpp"
#include "render/graph/framegraphbuilder.hpp"
#include "render/graph/framegraphblackboard.hpp"
#include "render/graph/framegraphpassresources.hpp"
#include "render/graph/framegraphrenderer.hpp"
#include "render/graph/framegraphresource.hpp"
#include "render/graph/framegraphpass.hpp"
#include "render/graph/framegraph.hpp"
#include "render/graph/framegraphpool.hpp"
#include "render/graph/gbuffer.hpp"
#include "render/graph/passes/skyboxpass.hpp"
#include "render/graph/passes/compositepass.hpp"
#include "render/graph/passes/phongpass.hpp"
#include "render/graph/passes/gbufferpass.hpp"
#include "asset/skybox.hpp"
#include "asset/audio.hpp"
#include "asset/light.hpp"
#include "asset/mesh.hpp"
#include "asset/vertex.hpp"
#include "asset/image.hpp"
#include "asset/sprite.hpp"
#include "asset/camera.hpp"
#include "asset/color.hpp"
#include "asset/shader.hpp"
#include "asset/texture.hpp"
#include "asset/material.hpp"
#include "asset/scene.hpp"
#include "event/eventbus.hpp"
#include "event/eventreceiver.hpp"
#include "text/font.hpp"
#include "text/textrenderer.hpp"
#include "text/character.hpp"
#include "text/text.hpp"
#include "async/threadpool.hpp"
#include "async/task.hpp"
#include "shader/spirvbundle.hpp"
#include "shader/shadercompiler.hpp"
#include "shader/spirvshader.hpp"
#include "shader/shaderlanguage.hpp"
#include "shader/shaderstage.hpp"
#include "shader/spirvblob.hpp"
#include "shader/shadersource.hpp"
#include "graphics/renderdeviceinfo.hpp"
#include "graphics/rendertarget.hpp"
#include "graphics/vertexbufferdesc.hpp"
#include "graphics/rendertargetdesc.hpp"
#include "graphics/graphicsdriver.hpp"
#include "graphics/vertexattribute.hpp"
#include "graphics/renderproperties.hpp"
#include "graphics/shaderprogramdesc.hpp"
#include "graphics/vertexbuffer.hpp"
#include "graphics/shaderbufferdesc.hpp"
#include "graphics/texturebuffer.hpp"
#include "graphics/textureproperties.hpp"
#include "graphics/shaderbuffer.hpp"
#include "graphics/renderobject.hpp"
#include "graphics/renderpass.hpp"
#include "graphics/renderpipelinedesc.hpp"
#include "graphics/shaderprogram.hpp"
#include "graphics/texturebufferdesc.hpp"
#include "graphics/renderpipeline.hpp"
#include "graphics/renderdevice.hpp"

#endif // XENGINE_ENGINE_HPP