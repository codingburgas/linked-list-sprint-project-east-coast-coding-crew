workspaceName = "Eccc"
workspace (workspaceName)
architecture "x86_64"
configurations { "Debug", "Release" }
project "Core"
  local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  local workSpaceBuildName = workspaceName
  local depFolderName = "dependencies"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++23"
  location ("./" .. workSpaceBuildName .. "/%{prj.name}")
  targetdir ("./bin/" .. outputdir .. "/%{prj.name}")
  objdir ("./bin-int/" .. outputdir .. "/%{prj.name}")
  files {
    "Eccc/Core/src/.cpp",
    "Eccc/Core/include/.h",
    "Eccc/Core/include/*.hpp"
  }
  includedirs {
    "./" .. depFolderName .. "/libpq/include",
    "./" .. depFolderName .. "/openssl/include",
    "./" .. depFolderName .. "/cpr/curl/include",
    "./" .. depFolderName .. "/cpr/include",
    "./" .. depFolderName .. "/fmt/include",
    "./" .. depFolderName .. "/ftxui/include",
    "./" .. depFolderName .. "/wfrest/base",
    "./" .. depFolderName .. "/wfrest/core",
    "./" .. depFolderName .. "/wfrest/util",
    "./" .. depFolderName .. "/nlohmann",
    "./" .. depFolderName .. "/sqlite_orm",
    "./" .. depFolderName .. "/websocketpp",
    "./" .. workSpaceBuildName .. "/%{prj.name}/include"
  }
  filter "configurations:"
    libdirs { "./" .. depFolderName .. "/libpq/lib", "./" .. depFolderName .. "/openssl/lib",
      "./" .. depFolderName .. "/fmt/src", 
      "./" .. depFolderName .. "/ftxui/src" ,
      "./" .. depFolderName .. "/libpq/include",
      "./" .. depFolderName .. "/openssl/include",
      "./" .. depFolderName .. "/cpr/curl/include",
      "./" .. depFolderName .. "/cpr/include",
      "./" .. depFolderName .. "/fmt/include",
      "./" .. depFolderName .. "/ftxui/include",
      "./" .. depFolderName .. "/wfrest/base",
      "./" .. depFolderName .. "/wfrest/core",
      "./" .. depFolderName .. "/wfrest/util",
      "./" .. depFolderName .. "/nlohmann",
      "./" .. depFolderName .. "/sqlite_orm",
      "./" .. depFolderName .. "/websocketpp",
    }
    links {
      "libpq",
      "libssl",
      "libcrypto",
      "fmt",
      "ftxui",
      "cpr"
    }
  systemversion "latest"
  filter "configurations:Debug"
    defines { "easteregg_DEBUG" }
    symbols "On"
    libdirs { 
        "./" .. depFolderName .. "/cpr/libs/Debug-libs", 
        "./" .. depFolderName .. "/fmt/src", 
        "./" .. depFolderName .. "/ftxui/src" ,
        "./" .. depFolderName .. "/libpq/include",
        "./" .. depFolderName .. "/openssl/include",
        "./" .. depFolderName .. "/cpr/curl/include",
        "./" .. depFolderName .. "/cpr/include",
        "./" .. depFolderName .. "/fmt/include",
        "./" .. depFolderName .. "/ftxui/include",
        "./" .. depFolderName .. "/wfrest/base",
        "./" .. depFolderName .. "/wfrest/core",
        "./" .. depFolderName .. "/wfrest/util",
        "./" .. depFolderName .. "/nlohmann",
        "./" .. depFolderName .. "/sqlite_orm",
        "./" .. depFolderName .. "/websocketpp",
    
        }
    links {
      "libcurl-d_imp",
      "cpr"
    }
    
  filter "configurations:Release"
    defines { "easteregg_RELEASE" }
    optimize "On"
    libdirs { "./" .. depFolderName .. "/cpr/libs/Release-libs", 
        "./" .. depFolderName .. "/fmt/src", 
        "./" .. depFolderName .. "/ftxui/src",
        "./" .. depFolderName .. "/ftxui/src" ,
        "./" .. depFolderName .. "/libpq/include",
        "./" .. depFolderName .. "/openssl/include",
        "./" .. depFolderName .. "/cpr/curl/include",
        "./" .. depFolderName .. "/cpr/include",
        "./" .. depFolderName .. "/fmt/include",
        "./" .. depFolderName .. "/ftxui/include",
        "./" .. depFolderName .. "/wfrest/base",
        "./" .. depFolderName .. "/wfrest/core",
        "./" .. depFolderName .. "/wfrest/util",
        "./" .. depFolderName .. "/nlohmann",
        "./" .. depFolderName .. "/sqlite_orm",
        "./" .. depFolderName .. "/websocketpp",
    }
    links {
      "libcurl_imp",
      "cpr"
    }