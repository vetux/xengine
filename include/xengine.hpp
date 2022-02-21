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

#include "script/script.hpp"
#include "script/mono/monocppvalue.hpp"
#include "script/mono/monoscript.hpp"
#include "script/mono/monocppdomain.hpp"
#include "script/mono/monocppobject.hpp"
#include "script/mono/monocppassembly.hpp"
#include "script/mono/monocpparguments.hpp"
#include "ecs/componentmanager.hpp"
#include "ecs/components.hpp"
#include "ecs/entity.hpp"
#include "ecs/entitymanager.hpp"
#include "ecs/system.hpp"
#include "ecs/ecs.hpp"
#include "ecs/componentpool.hpp"
#include "ecs/systems/animationsystem.hpp"
#include "ecs/systems/physics3dsystem.hpp"
#include "ecs/systems/rendersystem.hpp"
#include "ecs/systems/monoscriptingsystem.hpp"
#include "ecs/systems/audiosystem.hpp"
#include "ecs/systems/physics2dsystem.hpp"
#include "ecs/systems/eventsystem.hpp"
#include "ecs/components/monoscriptcomponent.hpp"
#include "ecs/components/meshrendercomponent.hpp"
#include "ecs/components/skyboxcomponent.hpp"
#include "ecs/components/cameracomponent.hpp"
#include "ecs/components/collidercomponent2d.hpp"
#include "ecs/components/particlecomponent.hpp"
#include "ecs/components/monosynccomponent.hpp"
#include "ecs/components/collidercomponent3d.hpp"
#include "ecs/components/rigidbodycomponent3d.hpp"
#include "ecs/components/transformcomponent.hpp"
#include "ecs/components/lightcomponent.hpp"
#include "ecs/components/rigidbodycomponent2d.hpp"
#include "ecs/components/gui/buttoncomponent.hpp"
#include "ecs/components/gui/textcomponent.hpp"
#include "ecs/components/gui/canvascomponent.hpp"
#include "ecs/components/audio/audiosourcecomponent.hpp"
#include "ecs/components/audio/audiolistenercomponent.hpp"
#include "display/monitor.hpp"
#include "display/displaybackend.hpp"
#include "display/windowlistener.hpp"
#include "display/windowattributes.hpp"
#include "display/videomode.hpp"
#include "display/displaymanager.hpp"
#include "display/window.hpp"
#include "physics/3d/rigidbody3d.hpp"
#include "physics/3d/world3d.hpp"
#include "physics/3d/collider3d.hpp"
#include "physics/3d/joint3d.hpp"
#include "physics/2d/rigidbody2d.hpp"
#include "physics/2d/world2d.hpp"
#include "physics/2d/collider2d.hpp"
#include "physics/2d/joint2d.hpp"
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
#include "audio/audiobackend.hpp"
#include "audio/audiobuffer.hpp"
#include "audio/audioformat.hpp"
#include "audio/audiolistener.hpp"
#include "audio/audiocontext.hpp"
#include "audio/audiodevice.hpp"
#include "io/pak.hpp"
#include "io/substreambuf.hpp"
#include "io/message.hpp"
#include "io/readfile.hpp"
#include "io/archive.hpp"
#include "io/protocol.hpp"
#include "io/byte.hpp"
#include "io/protocol/jsonprotocol.hpp"
#include "io/archive/pakarchive.hpp"
#include "io/archive/directoryarchive.hpp"
#include "crypto/aes.hpp"
#include "crypto/sha.hpp"
#include "app/application.hpp"
#include "input/inputlistener.hpp"
#include "input/input.hpp"
#include "input/device/keyboard.hpp"
#include "input/device/mouse.hpp"
#include "input/device/gamepad.hpp"
#include "compression/gzip.hpp"
#include "schema/ecsschema.hpp"
#include "schema/mathschema.hpp"
#include "render/pipeline.hpp"
#include "render/2d/renderer2d.hpp"
#include "render/deferred/deferredpipeline.hpp"
#include "render/deferred/compositor.hpp"
#include "render/deferred/renderpass.hpp"
#include "render/deferred/prepass.hpp"
#include "render/deferred/gbuffer.hpp"
#include "render/deferred/passes/forwardpass.hpp"
#include "render/deferred/passes/skyboxpass.hpp"
#include "render/deferred/passes/outlinepass.hpp"
#include "render/deferred/passes/phongpass.hpp"
#include "render/deferred/passes/shadowpass.hpp"
#include "render/deferred/passes/debugpass.hpp"
#include "render/forward/forwardrenderer.hpp"
#include "render/shader/shaderinclude.hpp"
#include "asset/assetbundle.hpp"
#include "asset/skybox.hpp"
#include "asset/assetexporter.hpp"
#include "asset/audio.hpp"
#include "asset/light.hpp"
#include "asset/mesh.hpp"
#include "asset/vertex.hpp"
#include "asset/image.hpp"
#include "asset/assetpath.hpp"
#include "asset/assetimporter.hpp"
#include "asset/camera.hpp"
#include "asset/color.hpp"
#include "asset/shader.hpp"
#include "asset/assethandle.hpp"
#include "asset/texture.hpp"
#include "asset/material.hpp"
#include "asset/asset.hpp"
#include "asset/scene.hpp"
#include "asset/manager/assetrendermanager.hpp"
#include "asset/manager/assetmanager.hpp"
#include "event/eventbus.hpp"
#include "event/eventreceiver.hpp"
#include "text/font.hpp"
#include "text/character.hpp"
#include "async/threadpool.hpp"
#include "async/task.hpp"
#include "render/shader/shadercompiler.hpp"
#include "render/shader/shaderlanguage.hpp"
#include "render/shader/shaderstage.hpp"
#include "render/shader/shadersource.hpp"
#include "render/platform/rendertarget.hpp"
#include "render/platform/graphicsbackend.hpp"
#include "render/platform/shaderbinary.hpp"
#include "render/platform/renderer.hpp"
#include "render/platform/meshbuffer.hpp"
#include "render/platform/texturebuffer.hpp"
#include "render/platform/renderobject.hpp"
#include "render/platform/renderallocator.hpp"
#include "render/platform/rendercommand.hpp"
#include "render/platform/shaderprogram.hpp"
#include "render/platform/renderdevice.hpp"

#endif // XENGINE_ENGINE_HPP