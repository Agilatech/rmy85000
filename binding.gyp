{
    "targets": [
        {
            "target_name": "rmy85000",
            "sources": [ "DataManip.cpp", "Device.cpp", "Rmy85000Drv.cpp", "Rmy85000Node.cpp" ],
            "cflags": ["-std=c++11", "-Wall"],
        }
    ],
    "link_settings": {
        "libraries": ["-pthread"]
    }
}

