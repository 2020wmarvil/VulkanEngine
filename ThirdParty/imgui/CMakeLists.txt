set(IMGUI_SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imstb_rectpack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imstb_textedit.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imstb_truetype.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_vulkan.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glfw.cpp"
)

target_sources(${ENGINE_NAME} PRIVATE ${IMGUI_SOURCE})
source_group("imgui" FILES ${IMGUI_SOURCE})