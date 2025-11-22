cmake_minimum_required(VERSION 3.23)

# Définir la famille STM32 avant tout
if(NOT DEFINED STM32_FAMILY)
    set(STM32_FAMILY "H7" CACHE STRING "STM32 Family (H7 or F4)")
    message(STATUS "STM32_FAMILY not defined, defaulting to: ${STM32_FAMILY}")
endif()

# Configuration du toolchain SEULEMENT si on est le projet racine
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Build standalone - configurer le toolchain
    if(NOT CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_SYSTEM_NAME Generic)
        set(CMAKE_SYSTEM_PROCESSOR arm)
        
        set(CMAKE_C_COMPILER arm-none-eabi-gcc)
        set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
        set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
        
        set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
        
        set(COMMON_FLAGS "-mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16")
        set(CMAKE_C_FLAGS_INIT "${COMMON_FLAGS} -fdata-sections -ffunction-sections")
        set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -fdata-sections -ffunction-sections")
        set(CMAKE_ASM_FLAGS_INIT "${COMMON_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS_INIT "${COMMON_FLAGS} -specs=nano.specs -Wl,--gc-sections")
    endif()
    
    project(stm32_components C CXX ASM)
else()
    # Inclus comme sous-projet - le parent a déjà configuré le compilateur
    message(STATUS "stm32-components: Building as subproject")
endif()

# Sélectionner le bon driver HAL selon la famille
if(STM32_FAMILY STREQUAL "H7")
    set(HAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/HAL/stm32h747-hal")
    
    if(NOT EXISTS "${HAL_LIB_PATH}/CMakeLists.txt")
        message(FATAL_ERROR 
            "STM32H7 HAL Driver submodule not found. Run:\n"
            " submodule-init-and-update script")
    endif()
    
    if(NOT DEFINED STM32_CHIP)
        set(STM32_CHIP "STM32H743xx" CACHE STRING "STM32 chip model")
    endif()
    
    add_subdirectory(${HAL_LIB_PATH})
    set(HAL_TARGET H7_COMMON)
    
elseif(STM32_FAMILY STREQUAL "F4")
    set(HAL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/HAL/stm32f412-hal")
    
    if(NOT EXISTS "${HAL_LIB_PATH}/CMakeLists.txt")
        message(FATAL_ERROR 
            "STM32F4 HAL Driver submodule not found. Run:\n"
            "  git submodule update --init HAL/stm32f412-hal")
    endif()
    
    if(NOT DEFINED STM32_CHIP)
        set(STM32_CHIP "STM32F412Zx" CACHE STRING "STM32 chip model")
    endif()
    
    add_subdirectory(${HAL_LIB_PATH})
    set(HAL_TARGET F4_COMMON)
    
else()
    message(FATAL_ERROR "Unsupported STM32_FAMILY: ${STM32_FAMILY}")
endif()

# Créer la librairie avec les sources
add_library(stm32_components STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Peripherals/Src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/Src/led.c
)

# Inclure les répertoires
target_include_directories(stm32_components PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Peripherals/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Sensors/Inc
)

# Lier avec le HAL approprié
target_link_libraries(stm32_components PUBLIC ${HAL_TARGET})

# Ajouter les définitions de compilation
if(STM32_FAMILY STREQUAL "H7")
    target_compile_definitions(stm32_components PUBLIC
        STM32H7
        USE_HAL_DRIVER
        CORE_CM7
        ${STM32_CHIP}
    )
elseif(STM32_FAMILY STREQUAL "F4")
    target_compile_definitions(stm32_components PUBLIC
        STM32F4
        USE_HAL_DRIVER
        ${STM32_CHIP}
    )
endif()