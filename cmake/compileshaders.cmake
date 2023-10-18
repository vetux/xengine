set(SHADER_HEADERS)

function(CompileShaderGLSL460 PATH STAGE ENTRYPOINT)
    add_custom_command(
            OUTPUT ${BASE_SOURCE_DIR}/shaders/generated/${PATH}.hpp
            DEPENDS shadercompiler ${BASE_SOURCE_DIR}/shaders/glsl_460/${PATH}.glsl
            COMMAND shadercompiler -m header -f -i ${BASE_SOURCE_DIR}/shaders/glsl_460_common/ -p -l GLSL_460 -s ${STAGE} -e main ${BASE_SOURCE_DIR}/shaders/glsl_460/${PATH}.glsl ${BASE_SOURCE_DIR}/shaders/generated/${PATH}.hpp)
    set(SHADER_HEADERS ${SHADER_HEADERS} ${BASE_SOURCE_DIR}/shaders/generated/${PATH}.hpp PARENT_SCOPE)
endfunction()

function(CompileShaderGLSLES320 PATH STAGE ENTRYPOINT)
    add_custom_command(
            OUTPUT ${BASE_SOURCE_DIR}/shaders/generated/${PATH}.hpp
            DEPENDS shadercompiler ${BASE_SOURCE_DIR}/shaders/glsl_320_es/${PATH}.glsl
            COMMAND shadercompiler -m header -f -i ${BASE_SOURCE_DIR}/shaders/glsl_320_es_common/ -p -l GLSL_ES_320 -s ${STAGE} -e main ${BASE_SOURCE_DIR}/shaders/glsl_320_es/${PATH}.glsl ${BASE_SOURCE_DIR}/shaders/generated/${PATH}.hpp)
    set(SHADER_HEADERS ${SHADER_HEADERS} ${BASE_SOURCE_DIR}/shaders/generated/${PATH}.hpp PARENT_SCOPE)
endfunction()

CompileShaderGLSL460(graph/constructionpass_vs VERTEX main)
CompileShaderGLSL460(graph/constructionpass_vs_skinned VERTEX main)
CompileShaderGLSL460(graph/constructionpass_fs FRAGMENT main)

CompileShaderGLSL460(graph/deferredlightingpass_vs VERTEX main)
CompileShaderGLSL460(graph/deferredlightingpass_fs FRAGMENT main)

CompileShaderGLSL460(graph/forwardlightingpass_vs VERTEX main)
CompileShaderGLSL460(graph/forwardlightingpass_fs FRAGMENT main)

CompileShaderGLSL460(graph/compositepass_vs VERTEX main)
CompileShaderGLSL460(graph/compositepass_fs FRAGMENT main)

CompileShaderGLSL460(ren2d/vs VERTEX main)
CompileShaderGLSL460(ren2d/fs FRAGMENT main)

CompileShaderGLSLES320(ren2d/vs_compat VERTEX main)
CompileShaderGLSLES320(ren2d/fs_compat FRAGMENT main)

CompileShaderGLSL460(testpass/vs VERTEX main)
CompileShaderGLSL460(testpass/fs FRAGMENT main)

CompileShaderGLSL460(mandelbrot/mandelbrot COMPUTE main)

list(LENGTH SHADER_HEADERS LEN_SHADER_HEADERS)

message("${LEN_SHADER_HEADERS} Precompiled Shaders")
foreach (val IN LISTS SHADER_HEADERS)
    message("   ${val}")
endforeach ()