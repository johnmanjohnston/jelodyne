cmake_minimum_required(VERSION 3.15)
project(PhaseVocoder VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(JUCE)

juce_add_gui_app(jelodyne
    BUNDLE_ID                       com.johnmanjohnston.jelodyne
    NEEDS_CURL                      FALSE
    NEEDS_WEB_BROWSER               FALSE
    MICROPHONE_PERMISSION_ENABLED   FALSE
    CAMERA_PERMISSION_ENABLED       FALSE
    BLUETOOTH_PERMISSION_ENABLED    FALSE
    FILE_SHARING_ENABLED            FALSE
    DOCUMENT_BROWSER_ENABLED        FALSE
    REQUIRES_FULL_SCREEN            FALSE
    IPAD_SCREEN_ORIENTATIONS        UIInterfaceOrientationPortrait
                                    UIInterfaceOrientationPortraitUpsideDown
                                    UIInterfaceOrientationLandscapeLeft
                                    UIInterfaceOrientationLandscapeRight
    IPHONE_SCREEN_ORIENTATIONS      UIInterfaceOrientationPortrait
                                    UIInterfaceOrientationPortraitUpsideDown
                                    UIInterfaceOrientationLandscapeLeft
                                    UIInterfaceOrientationLandscapeRight)

juce_generate_juce_header(jelodyne)
target_sources(jelodyne PRIVATE
    src/main.cpp
    src/mainComponent.cpp
    src/clip.cpp
    src/piano.cpp
    src/note.cpp
    src/utility.cpp
    src/noteComponent.cpp
    src/jlistener/jlistenerbroadcaster.cpp
    src/shift.cpp
    src/scale.cpp
    rubberband/single/RubberBandSingle.cpp)


target_compile_definitions(jelodyne PRIVATE
    PIP_JUCE_EXAMPLES_DIRECTORY_STRING="${JUCE_SOURCE_DIR}/examples"
    JUCE_ALLOW_STATIC_NULL_VARIABLES=0
    JUCE_CONTENT_SHARING=0
    JUCE_DEMO_RUNNER=0
    JUCE_PLUGINHOST_LV2=0
    JUCE_PLUGINHOST_VST3=0
    JUCE_STRICT_REFCOUNTEDPOINTER=0
    JUCE_UNIT_TESTS=0
    JUCE_USE_CAMERA=0
    JUCE_USE_CURL=0
    JUCE_USE_MP3AUDIOFORMAT=0
    JUCE_WEB_BROWSER=0
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS/iOS.
    JUCE_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(jelodyne PRIVATE
    juce::juce_analytics
    juce::juce_animation
    juce::juce_audio_utils
    juce::juce_box2d
    juce::juce_dsp
    juce::juce_opengl
    juce::juce_osc
    juce::juce_product_unlocking
    juce::juce_video
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
