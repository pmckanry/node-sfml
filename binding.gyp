{
    "targets": [
        {
            "target_name": "sfml-node",
            "sources": [ 
                "src/sfml-node.cc"
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