#version 460

#include "phong.glsl"
#include "pbr.glsl"

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUv;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 boneWeights;

layout(location = 0) out vec3 fPos;
layout(location = 1) out vec3 fNorm;
layout(location = 2) out vec3 fTan;
layout(location = 3) out vec2 fUv;
layout(location = 4) out vec4 vPos;
layout(location = 5) out vec3 fT;
layout(location = 6) out vec3 fB;
layout(location = 7) out vec3 fN;
layout(location = 8) flat out uint drawID;

struct ShaderAtlasTexture {
    ivec4 level_index_filtering_assigned;
    vec4 atlasScale_texSize;
};

struct ShaderDrawData {
    mat4 model;
    mat4 mvp;

    ivec4 objectID_shadows;

    vec4 metallic_roughness_ambientOcclusion;

    vec4 albedoColor;

    vec4 normalIntensity;

    ShaderAtlasTexture normal;

    ShaderAtlasTexture metallic;
    ShaderAtlasTexture roughness;
    ShaderAtlasTexture ambientOcclusion;

    ShaderAtlasTexture albedo;
};

layout(binding = 0, std140) buffer ShaderUniformBuffer
{
    vec4 viewPosition;
    vec4 viewportSize;
    ShaderDrawData data[];
} globs;
layout(binding = 1) uniform sampler2DArray atlasTextures[12];

layout(binding = 13) uniform sampler2D deferredDepth;

layout(binding = 14, std140) buffer PointLightsData
{
    PBRPointLight lights[];
} pointLights;

layout(binding = 15, std140) buffer PointLightsDataShadow
{
    PBRPointLight lights[];
} pointLightsShadow;

layout(binding = 16) uniform samplerCubeArray pointLightShadowMaps;

layout(binding = 17, std140) buffer DirectionalLightsData
{
    PBRDirectionalLight lights[];
} directionalLights;

layout(binding = 18, std140) buffer ShadowDirectionalLightsData
{
    PBRDirectionalLight lights[];
} directionalLightsShadow;

layout(binding = 19, std140) buffer SpotLightsData
{
    PBRSpotLight lights[];
} spotLights;

layout(binding = 20, std140) buffer ShadowSpotLightsData
{
    PBRSpotLight lights[];
} spotLightsShadow;

void main()
{
    ShaderDrawData data = globs.data[gl_DrawID];

    vPos = data.mvp * vec4(vPosition, 1);
    fPos = (data.model * vec4(vPosition, 1)).xyz;
    fUv = vUv;

    fNorm = normalize(vNormal);
    fTan = normalize(vTangent);

    //https://www.gamedeveloper.com/programming/three-normal-mapping-techniques-explained-for-the-mathematically-uninclined
    fN = normalize((data.model * vec4(vNormal, 0.0)).xyz);
    fT = normalize((data.model * vec4(vTangent, 0.0)).xyz);
    fB = normalize((data.model * vec4(cross(vNormal, vTangent.xyz) * 1, 0.0)).xyz);

    gl_Position = vPos;

    drawID = gl_DrawID;
}
