
  workspace "eccc"
  architecture "x86_64"
  configurations { "Debug", "Release" }
  project "core"
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    location "./eccc/%{prj.name}"
    targetdir ("./bin/" .. outputdir .. "/%{prj.name}")
    objdir ("./bin-int/" .. outputdir .. "/%{prj.name}")
    files {
      "eccc/core/src/**.cpp",
      "eccc/core/include/**.h",
      "eccc/core/include/**.hpp",
      "./vendor/fmt/src/**.cpp",
      "./vendor/fmt/src/**.c",
      "./vendor/fmt/src/**.h",
      "eccc/core/include/**.hpp"
    }
    includedirs {
      "./vendor/libpq/include",
      "./vendor/openssl/include",
      "./vendor/cpr/curl/include",
      "./vendor/cpr/include",
      "./vendor/fmt/include",
      "./vendor/wfrest/base",
      "./vendor/wfrest/core",
      "./vendor/wfrest/util",
      "./vendor/fmt/src", 
      "./vendor/nlohmann",
      "./vendor/sqlite_orm",
      "./vendor/websocketpp",
      "./vendor/libpq/include", 
      "./vendor/openssl/include", 
      "./vendor/soci/include",
      "./Eccc/%{prj.name}/include"
    }
    filter "configurations:*"
      libdirs { "./vendor/libpq/lib", "./vendor/openssl/lib",
      }
      links {
        "libpq",
        "libssl",
        "libcrypto",
        "cpr"
      }
    systemversion "latest"
    filter "configurations:Debug"
      defines { "easteregg_DEBUG" }
      symbols "On"
      libdirs { "./vendor/soci/lib/debug_libs", "./vendor/cpr/libs/Debug-libs" }
      links {
        "libsoci_core_4_0",
        "libsoci_empty_4_0",
        "libsoci_postgresql_4_0",
        "libcurl-d_imp",
        "cpr"
      }
      
    filter "configurations:Release"
      defines { "easteregg_RELEASE" }
      optimize "On"
      libdirs { "./vendor/soci/lib", "./vendor/cpr/libs/Release-libs" }
      links {	
        "libsoci_core_4_0",
        "libsoci_empty_4_0",
        "libsoci_postgresql_4_0",
        "libcurl_imp",
        "cpr"
      }