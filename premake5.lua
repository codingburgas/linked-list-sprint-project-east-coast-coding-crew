workspace "eccc"
  architecture "x86_64"
  configurations { "Debug", "Release" }
  startproject "core"
  
  -- Define common path variables
  outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
  -- Common include directories used by all projects
  common_includes = {
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
    "./Eccc/core/include"
  }
  
  -- Common library directories
  function setup_libdirs(config)
    if config == "Debug" then
      libdirs { 
        "./vendor/libpq/lib", 
        "./vendor/openssl/lib",
        "./vendor/soci/lib/debug_libs", 
        "./vendor/cpr/libs/Debug-libs" 
      }
    else
      libdirs { 
        "./vendor/libpq/lib", 
        "./vendor/openssl/lib",
        "./vendor/soci/lib", 
        "./vendor/cpr/libs/Release-libs" 
      }
    end
  end
  
  -- Common libraries to link
  function setup_links(config)
    links {
      "libpq",
      "libssl",
      "libcrypto",
      "cpr"
    }
    
    if config == "Debug" then
      links {
        "libsoci_core_4_0",
        "libsoci_empty_4_0",
        "libsoci_postgresql_4_0",
        "libcurl-d_imp"
      }
    else
      links {
        "libsoci_core_4_0",
        "libsoci_empty_4_0",
        "libsoci_postgresql_4_0",
        "libcurl_imp"
      }
    end
  end
  
  -- Main project
  project "core"
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
    }
    
    includedirs(common_includes)
    
    filter "configurations:*"
      libdirs { "./vendor/libpq/lib", "./vendor/openssl/lib" }
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
      setup_libdirs("Debug")
      setup_links("Debug")
      
    filter "configurations:Release"
      defines { "easteregg_RELEASE" }
      optimize "On"
      setup_libdirs("Release")
      setup_links("Release")

  -- Unit test projects
  -- Historical Linked List Test
  project "historical_linked_list_test"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    location "./eccc/core/tests"
    targetdir ("./bin/" .. outputdir .. "/tests")
    objdir ("./bin-int/" .. outputdir .. "/tests/%{prj.name}")
    
    files {
      "eccc/core/tests/historical_linked_list_test.cpp",
      "eccc/core/src/historical_linked_list.cpp",
    }
    
    includedirs(common_includes)
    
    filter "configurations:*"
      libdirs { "./vendor/libpq/lib", "./vendor/openssl/lib" }
      links {
        "libpq",
        "libssl",
        "libcrypto",
      }
    
    systemversion "latest"
    
    filter "configurations:Debug"
      defines { "easteregg_DEBUG" }
      symbols "On"
      setup_libdirs("Debug")
      
    filter "configurations:Release"
      defines { "easteregg_RELEASE" }
      optimize "On"
      setup_libdirs("Release")
  
  -- Database Test
  project "database_test"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    location "./eccc/core/tests"
    targetdir ("./bin/" .. outputdir .. "/tests")
    objdir ("./bin-int/" .. outputdir .. "/tests/%{prj.name}")
    
    files {
      "eccc/core/tests/database_test.cpp",
      "eccc/core/src/database.cpp",
    }
    
    includedirs(common_includes)
    
    filter "configurations:*"
      libdirs { "./vendor/libpq/lib", "./vendor/openssl/lib" }
      links {
        "libpq",
        "libssl",
        "libcrypto",
      }
    
    systemversion "latest"
    
    filter "configurations:Debug"
      defines { "easteregg_DEBUG" }
      symbols "On"
      setup_libdirs("Debug")
      setup_links("Debug")
      
    filter "configurations:Release"
      defines { "easteregg_RELEASE" }
      optimize "On"
      setup_libdirs("Release")
      setup_links("Release")
  
  -- Unit Tests Runner
  project "unitTests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    location "./eccc/core/tests"
    targetdir ("./bin/" .. outputdir .. "/tests")
    objdir ("./bin-int/" .. outputdir .. "/tests/%{prj.name}")
    
    files {
      "eccc/core/tests/run_tests.cpp",
    }
    
    includedirs(common_includes)
    
    systemversion "latest"
    
    filter "configurations:Debug"
      defines { "easteregg_DEBUG" }
      symbols "On"
      
    filter "configurations:Release"
      defines { "easteregg_RELEASE" }
      optimize "On"

  -- Add custom build steps for Windows to copy DLLs to target directory
  filter { "system:windows" }
    postbuildcommands {
      "{COPYDIR} \"%{wks.location}/vendor/libpq/bin\" \"%{cfg.targetdir}\"",
      "{COPYDIR} \"%{wks.location}/vendor/openssl/bin\" \"%{cfg.targetdir}\""
    }