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

#ifndef XENGINE_ENGINE_HPP
#define XENGINE_ENGINE_HPP

#include "script/script.hpp"
#include "script/scriptruntime.hpp"
#include "script/scriptdriver.hpp"
#include "ecs/componentregistry.hpp"
#include "ecs/entityhandle.hpp"
#include "ecs/components.hpp"
#include "ecs/entity.hpp"
#include "ecs/systemregistry.hpp"
#include "ecs/system.hpp"
#include "ecs/ecs.hpp"
#include "ecs/componentpool.hpp"
#include "ecs/entityscene.hpp"
#include "ecs/systems/meshrendersystem.hpp"
#include "ecs/systems/spriteanimationsystem.hpp"
#include "ecs/systems/audiosystem.hpp"
#include "ecs/systems/canvasrendersystem.hpp"
#include "ecs/systems/physicssystem.hpp"
#include "ecs/systems/guieventsystem.hpp"
#include "ecs/systems/eventsystem.hpp"
#include "ecs/components/meshrendercomponent.hpp"
#include "ecs/components/skyboxcomponent.hpp"
#include "ecs/components/cameracomponent.hpp"
#include "ecs/components/rigidbodycomponent.hpp"
#include "ecs/components/buttoncomponent.hpp"
#include "ecs/components/particlecomponent.hpp"
#include "ecs/components/textcomponent.hpp"
#include "ecs/components/spritecomponent.hpp"
#include "ecs/components/audiosourcecomponent.hpp"
#include "ecs/components/canvascomponent.hpp"
#include "ecs/components/audiolistenercomponent.hpp"
#include "ecs/components/canvastransformcomponent.hpp"
#include "ecs/components/spriteanimationcomponent.hpp"
#include "ecs/components/transformcomponent.hpp"
#include "ecs/components/lightcomponent.hpp"
#include "display/monitor.hpp"
#include "display/displaydriver.hpp"
#include "display/windowlistener.hpp"
#include "display/windowattributes.hpp"
#include "display/videomode.hpp"
#include "display/window.hpp"
#include "physics/collider.hpp"
#include "physics/world.hpp"
#include "physics/collidershape.hpp"
#include "physics/joint.hpp"
#include "physics/colliderdesc.hpp"
#include "physics/physicsdriver.hpp"
#include "physics/rigidbody.hpp"
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
#include "audio/audiosource.hpp"
#include "audio/audiobuffer.hpp"
#include "audio/audioformat.hpp"
#include "audio/audiolistener.hpp"
#include "audio/audiodriver.hpp"
#include "audio/audiocontext.hpp"
#include "audio/audiodevice.hpp"
#include "driver/driver.hpp"
#include "driver/driverregistry.hpp"
#include "driver/registerdriver.hpp"
#include "gpu/renderdeviceinfo.hpp"
#include "gpu/rendertarget.hpp"
#include "gpu/vertexbufferdesc.hpp"
#include "gpu/gpudriver.hpp"
#include "gpu/rendertargetdesc.hpp"
#include "gpu/vertexattribute.hpp"
#include "gpu/renderproperties.hpp"
#include "gpu/shaderprogramdesc.hpp"
#include "gpu/vertexbuffer.hpp"
#include "gpu/shaderbufferdesc.hpp"
#include "gpu/texturebuffer.hpp"
#include "gpu/textureproperties.hpp"
#include "gpu/shaderbuffer.hpp"
#include "gpu/renderobject.hpp"
#include "gpu/rendercommand.hpp"
#include "gpu/shaderbinding.hpp"
#include "gpu/renderpipelinedesc.hpp"
#include "gpu/shaderprogram.hpp"
#include "gpu/gpufence.hpp"
#include "gpu/texturebufferdesc.hpp"
#include "gpu/renderpipeline.hpp"
#include "gpu/renderdevice.hpp"
#include "gpu/compute/computepipelinedesc.hpp"
#include "gpu/compute/computepipeline.hpp"
#include "gpu/compute/computecommand.hpp"
#include "gpu/raytrace/raytracepipeline.hpp"
#include "gpu/raytrace/raytracecommand.hpp"
#include "gpu/raytrace/raytracepipelinedesc.hpp"
#include "util/numeric_cast.hpp"
#include "util/refcounter.hpp"
#include "util/hashcombine.hpp"
#include "util/listenable.hpp"
#include "util/crc.hpp"
#include "util/counter.hpp"
#include "io/messageable.hpp"
#include "io/pak.hpp"
#include "io/substreambuf.hpp"
#include "io/message.hpp"
#include "io/readfile.hpp"
#include "io/archive.hpp"
#include "io/pakbuilder.hpp"
#include "io/library.hpp"
#include "io/protocol.hpp"
#include "io/byte.hpp"
#include "io/protocol/jsonprotocol.hpp"
#include "io/archive/pakarchive.hpp"
#include "io/archive/directoryarchive.hpp"
#include "io/archive/memoryarchive.hpp"
#include "resource/resourceimporter.hpp"
#include "resource/resourcebundle.hpp"
#include "resource/resourceregistry.hpp"
#include "resource/resourceparser.hpp"
#include "resource/resourceexporter.hpp"
#include "resource/resourcehandle.hpp"
#include "resource/uri.hpp"
#include "resource/resource.hpp"
#include "resource/parsers/jsonparser.hpp"
#include "resource/parsers/stbiparser.hpp"
#include "crypto/aes.hpp"
#include "crypto/gzip.hpp"
#include "crypto/sha.hpp"
#include "crypto/random.hpp"
#include "crypto/cryptodriver.hpp"
#include "app/application.hpp"
#include "types/deltatime.hpp"
#include "input/inputlistener.hpp"
#include "input/input.hpp"
#include "input/device/keyboard.hpp"
#include "input/device/mouse.hpp"
#include "input/device/gamepad.hpp"
#include "input/device/buttonstate.hpp"
#include "animation/skeletal/rig.hpp"
#include "animation/skeletal/riganimator.hpp"
#include "animation/skeletal/bone.hpp"
#include "animation/skeletal/riganimation.hpp"
#include "animation/skeletal/boneanimation.hpp"
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
#include "asset/rawasset.hpp"
#include "asset/skybox.hpp"
#include "asset/audio.hpp"
#include "asset/light.hpp"
#include "asset/mesh.hpp"
#include "asset/primitive.hpp"
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
#include "event/event.hpp"
#include "event/eventlistener.hpp"
#include "event/events/inputevent.hpp"
#include "event/events/contactevent.hpp"
#include "event/events/entityevent.hpp"
#include "event/events/windowevent.hpp"
#include "event/events/guievent.hpp"
#include "text/font.hpp"
#include "text/textrenderer.hpp"
#include "text/fontdriver.hpp"
#include "text/textrenderproperties.hpp"
#include "text/alignment.hpp"
#include "text/character.hpp"
#include "text/text.hpp"
#include "async/threadpool.hpp"
#include "async/task.hpp"
#include "shader/spirvcompiler.hpp"
#include "shader/spirvbundle.hpp"
#include "shader/spirvshader.hpp"
#include "shader/shaderlanguage.hpp"
#include "shader/shaderstage.hpp"
#include "shader/spirvdecompiler.hpp"
#include "shader/spirvblob.hpp"
#include "shader/shadersource.hpp"

#endif // XENGINE_ENGINE_HPP