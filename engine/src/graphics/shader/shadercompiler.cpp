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

#include "graphics/shader/shadercompiler.hpp"

#include <stdexcept>

#include "shaderc/shaderc.hpp"

#include "spirv_glsl.hpp"
#include "spirv_hlsl.hpp"

namespace xengine {
    class IncludeHandler : public shaderc::CompileOptions::IncluderInterface {
    public:
        std::function<std::string(const char *)> callback;

        IncludeHandler() = default;

        explicit IncludeHandler(std::function<std::string(const char *)> callback)
                : callback(std::move(callback)) {}

        shaderc_include_result *GetInclude(const char *requested_source,
                                           shaderc_include_type type,
                                           const char *requesting_source,
                                           size_t include_depth) override {
            auto *ret = new shaderc_include_result();
            auto userData = new std::pair<std::string, std::string>(std::string(callback(requested_source)),
                                                                    std::string(requested_source));
            ret->user_data = userData;
            ret->content = userData->first.c_str();
            ret->content_length = userData->first.length();
            ret->source_name = userData->second.c_str();
            ret->source_name_length = userData->second.length();

            return ret;
        }

        void ReleaseInclude(shaderc_include_result *data) override {
            auto *pair = static_cast<std::pair<std::string, std::string> *>(data->user_data);
            delete pair;
            delete data;
        }
    };

    static shaderc_optimization_level convertOptimizationLevel(ShaderCompiler::OptimizationLevel opt) {
        switch (opt) {
            case ShaderCompiler::OPTIMIZATION_NONE:
                return shaderc_optimization_level_zero;
            case ShaderCompiler::OPTIMIZATION_PERFORMANCE:
                return shaderc_optimization_level_performance;
            case ShaderCompiler::OPTIMIZATION_SIZE:
                return shaderc_optimization_level_size;
            default:
                throw std::runtime_error("Invalid optimization level " + std::to_string(opt));
        }
    }

    static spv::ExecutionModel convertShaderStage(ShaderStage stage) {
        switch (stage) {
            case VERTEX:
                return spv::ExecutionModel::ExecutionModelVertex;
            case GEOMETRY:
                return spv::ExecutionModel::ExecutionModelGeometry;
            case FRAGMENT:
                return spv::ExecutionModel::ExecutionModelFragment;
            default:
                throw std::runtime_error("Unsupported shader stage");
        }
    }

    std::vector<uint32_t> ShaderCompiler::compileToSPIRV(const std::string &source,
                                                         const std::string &entryPoint,
                                                         ShaderStage stage,
                                                         ShaderLanguage language,
                                                         OptimizationLevel optimizationLevel) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        shaderc_shader_kind shaderStage;
        switch (stage) {
            case VERTEX:
                shaderStage = shaderc_vertex_shader;
                break;
            case GEOMETRY:
                shaderStage = shaderc_geometry_shader;
                break;
            case FRAGMENT:
                shaderStage = shaderc_fragment_shader;
                break;
        }

        shaderc_source_language shaderLang;
        switch (language) {
            case HLSL_SHADER_MODEL_4:
                shaderLang = shaderc_source_language_hlsl;
                break;
            case GLSL_410:
                options.SetTargetEnvironment(shaderc_target_env_opengl, 410);
                shaderLang = shaderc_source_language_glsl;
                break;
            case GLSL_410_VK:
                options.SetTargetEnvironment(shaderc_target_env_vulkan, 410);
                shaderLang = shaderc_source_language_glsl;
                break;
            case GLSL_ES_320:
                options.SetTargetEnvironment(shaderc_target_env_opengl, 320);
                shaderLang = shaderc_source_language_glsl;
                break;
        }

        options.SetSourceLanguage(shaderLang);
        options.SetAutoBindUniforms(false);
        options.SetAutoSampledTextures(true);
        options.SetAutoMapLocations(true);
        options.SetOptimizationLevel(convertOptimizationLevel(optimizationLevel));

        auto compileResult = compiler.CompileGlslToSpv(source,
                                                       shaderStage,
                                                       "shader",
                                                       entryPoint.c_str(),
                                                       options);

        if (compileResult.GetCompilationStatus() != shaderc_compilation_status_success) {
            throw std::runtime_error("Failed to compile to spirv " + compileResult.GetErrorMessage());
        }

        return {compileResult.cbegin(), compileResult.cend()};
    }

    std::string ShaderCompiler::decompileSPIRV(const std::vector<uint32_t> &source,
                                               const std::string &entryPoint,
                                               ShaderStage stage,
                                               ShaderLanguage targetLanguage) {
        switch (targetLanguage) {
            case HLSL_SHADER_MODEL_4: {
                spirv_cross::CompilerHLSL sCompiler(source);
                sCompiler.set_entry_point(entryPoint, convertShaderStage(stage));

                spirv_cross::ShaderResources resources = sCompiler.get_shader_resources();

                spirv_cross::CompilerGLSL::Options sOptions;
                spirv_cross::CompilerHLSL::Options hlslOptions;

                hlslOptions.shader_model = 40;
                sCompiler.set_hlsl_options(hlslOptions);
                sCompiler.set_common_options(sOptions);

                sCompiler.build_combined_image_samplers();

                return sCompiler.compile();
            }
            case GLSL_410_VK:
            case GLSL_410: {
                spirv_cross::CompilerGLSL sCompiler(source);
                sCompiler.set_entry_point(entryPoint, convertShaderStage(stage));

                spirv_cross::ShaderResources resources = sCompiler.get_shader_resources();

                spirv_cross::CompilerGLSL::Options sOptions;
                sOptions.version = 410;

                //Dont generate glsl which uses the uniform buffer api.
                sOptions.emit_uniform_buffer_as_plain_uniforms = true;
                sOptions.separate_shader_objects = true;

                sCompiler.set_common_options(sOptions);

                sCompiler.build_dummy_sampler_for_combined_images();
                sCompiler.build_combined_image_samplers();

                return sCompiler.compile();
            }
            case GLSL_ES_320 : {
                spirv_cross::CompilerGLSL sCompiler(source);
                sCompiler.set_entry_point(entryPoint, convertShaderStage(stage));

                spirv_cross::ShaderResources resources = sCompiler.get_shader_resources();

                spirv_cross::CompilerGLSL::Options sOptions;
                sOptions.version = 320;

                //Dont generate glsl which uses the uniform buffer api.
                sOptions.emit_uniform_buffer_as_plain_uniforms = true;
                sOptions.separate_shader_objects = true;

                sOptions.es = true;

                //Set medium precision because high precision is optional in OpenGL ES
                sOptions.fragment.default_float_precision = sOptions.Mediump;
                sOptions.fragment.default_int_precision = sOptions.Mediump;

                sCompiler.set_common_options(sOptions);

                return sCompiler.compile();
            }
            default:
                throw std::runtime_error("");
        }
    }

    static void stripLines(std::string &str, const std::string &needle) {
        auto it = str.find(needle);
        while (it != std::string::npos) {
            auto eol = str.find('\n', it);
            str.erase(it, eol - it);
            it = str.find(needle);
        }
    }

    std::string ShaderCompiler::preprocess(const std::string &source,
                                           ShaderStage stage,
                                           ShaderLanguage language,
                                           const std::function<std::string(const char *)> &include,
                                           const std::map<std::string, std::string> &macros,
                                           OptimizationLevel optimizationLevel) {
        shaderc_shader_kind shaderStage;
        switch (stage) {
            case VERTEX:
                shaderStage = shaderc_vertex_shader;
                break;
            case GEOMETRY:
                shaderStage = shaderc_geometry_shader;
                break;
            case FRAGMENT:
                shaderStage = shaderc_fragment_shader;
                break;
        }

        shaderc_source_language shaderLang;
        switch (language) {
            case HLSL_SHADER_MODEL_4:
                shaderLang = shaderc_source_language_hlsl;
                break;
            case GLSL_410:
            case GLSL_410_VK:
            case GLSL_ES_320:
                shaderLang = shaderc_source_language_glsl;
                break;
        }

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        for (auto &p: macros)
            options.AddMacroDefinition(p.first, p.second);

        options.SetIncluder(std::make_unique<IncludeHandler>(include));
        options.SetSourceLanguage(shaderLang);
        options.SetAutoBindUniforms(false);
        options.SetAutoSampledTextures(true);
        options.SetAutoMapLocations(true);

        options.SetOptimizationLevel(convertOptimizationLevel(optimizationLevel));

        auto preProcessResult = compiler.PreprocessGlsl(source,
                                                        shaderStage,
                                                        "shader",
                                                        options);

        auto status = preProcessResult.GetCompilationStatus();

        if (status != shaderc_compilation_status_success) {
            throw std::runtime_error("Failed to preprocess glsl: " + preProcessResult.GetErrorMessage());
        }

        std::string ret = {preProcessResult.cbegin(), preProcessResult.cend()};

        if (language != HLSL_SHADER_MODEL_4) {
            //Remove useless directives which break compilation of the preprocessed glsl.
            stripLines(ret, "#extension GL_GOOGLE_include_directive");
            stripLines(ret, "#line");
        }

        return ret;
    }
}