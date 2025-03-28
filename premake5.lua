workspaceName = "East-Coast-Coding-Crew"
workspace(workspaceName)

architecture "x86_64"

configurations { "Debug", "Release" }

project "Core"
  local outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  local workSpaceBuildName = "eccc"
  local depFolderName = "dependencies"

  kind "ConsoleApp"
  
  language "C++"
  cppdialect "C++Latest"

  location ("./" .. workSpaceBuildName .. "/%{prj.name}")

  targetdir ("./bin/" .. outputdir .. "/%{prj.name}")
  objdir ("./bin-int/" .. outputdir .. "/%{prj.name}")

  files { "./" .. outputdir .. "/%{prj.name}/src/**.cpp", "./" .. outputdir .. "/%{prj.name}/include/**.h", "./" .. outputdir .. "/%{prj.name}/include/**.hpp" }

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

  filter "configurations:*"
    libdirs { "./" .. depFolderName .. "/libpq/lib", "./" .. depFolderName .. "/openssl/lib" }
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
    libdirs { "./" .. depFolderName .. "/cpr/libs/Debug-libs", "./" .. depFolderName .. "/fmt/src", "./" .. depFolderName .. "/ftxui/src" }
    links { 
      "libcurl-d_imp",
      "cpr"
    }
  
  filter "configurations:Release"
    defines { "easteregg_RELEASE" }
    optimize "On"
    libdirs { "./" .. depFolderName .. "/cpr/libs/Release-libs", "./" .. depFolderName .. "/fmt/src", "./" .. depFolderName .. "/ftxui/src" }
    links { 
      "libcurl_imp",
      "cpr"
    }