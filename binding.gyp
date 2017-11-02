{
    "targets": [
        {
            "target_name": "sfml-node",
            "sources": [ 
                "src/sfml-node.cc",
                "src/audio/music.cc",
                "src/audio/sound.cc",
                "src/audio/soundbuffer.cc",
                "src/graphics/font.cc",
                "src/graphics/image.cc",
                "src/graphics/renderwindow.cc",
                "src/graphics/sprite.cc",
                "src/graphics/text.cc",
                "src/graphics/texture.cc",
                "src/graphics/view.cc",
                "src/system/clock.cc",
                "src/window/window.cc",
            ],
            "include_dirs": [
                "/usr/local/include/SFML",
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")"
            ],
            "libraries": [
                "-lcsfml-system", 
                "-lcsfml-window", 
                "-lcsfml-graphics", 
                "-lcsfml-audio", 
                "-lcsfml-network"
            ],
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "xcode_settings": {
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "CLANG_CXX_LIBRARY": "libc++",
                "MACOSX_DEPLOYMENT_TARGET": "10.7",
            },
            "msvs_settings": {
                "VCCLCompilerTool": { "ExceptionHandling": 1 },
            },
            "defines": [
                "NAPI_CPP_EXCEPTIONS"
            ]
        }
    ]
}