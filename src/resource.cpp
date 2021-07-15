#include "resource.h"
#include "runtime.h"
#include <string>

#ifdef SERVER_MODULE
    // For filestream
    #include <fstream>
    #include <streambuf>
#endif

bool BoilerplateResource::Start()
{
    // Load file
    auto src = ReadFile(resource->GetMain());
    if(src.IsEmpty())
    {
        Log::Error << "Failed to read resource main file" << Log::Endl;
        return false;
    }

    // Now we can access the resource main file and e.g. start a script using the source code
    // To showcase this, we are just going to log the file content here
    Log::Info << "Resource main content:" << Log::Endl;
    Log::Info << src << Log::Endl;

    return true;
}

bool BoilerplateResource::Stop()
{
    // Do everything here that needs cleanup after the resource has been stopped
    // Make sure to clean up EVERYTHING here that the resource has created,
    // so that dynamic resource restarts are possible
    return true;
}

bool BoilerplateResource::OnEvent(const alt::CEvent* ev)
{
    // Handle incoming events here, e.g. call the event handlers registered by the resource
    return true;
}

void BoilerplateResource::OnTick()
{
    // Do everything here that needs to be handled on tick that is resource related
    // This can be e.g. timers
}

void BoilerplateResource::OnCreateBaseObject(alt::Ref<alt::IBaseObject> object)
{
    // Called every time a base object has been created, so you can use this to keep track
    // of all the existing base objects, to check if they are valid in the user scripts
}

void BoilerplateResource::OnRemoveBaseObject(alt::Ref<alt::IBaseObject> object)
{
    // Called every time a base object has been created, so you can use this to keep track
    // of all the existing base objects, to check if they are valid in the user scripts
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define FILE_SEPERATOR "\\"
#else
    #define FILE_SEPERATOR "/"
#endif
alt::String BoilerplateResource::ReadFile(alt::String path)
{
    // Use the alt:V functions to read the file
#ifdef CLIENT_MODULE
    auto pkg = resource->GetPackage();
    // Check if file exists
    if(!pkg->FileExists(path)) return alt::String();
    // Open file
    alt::IPackage::File* pkgFile = pkg->OpenFile(path);
    alt::String          src(pkg->GetFileSize(pkgFile));
    // Read file content
    pkg->ReadFile(pkgFile, src.GetData(), src.GetSize());
    pkg->CloseFile(pkgFile);

    return src;
#endif  // CLIENT_MODULE

    // Use the native C++ API to read the file, because it's faster
#ifdef SERVER_MODULE
    // Open file
    std::ifstream file((resource->GetPath() + FILE_SEPERATOR + path).CStr());
    if(!file.good()) return alt::String();
    // Read file content
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return str;
#endif  // SERVER_MODULE
}
