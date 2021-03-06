#include <vector>
#include <fstream>
#include <SDL2/SDL_log.h>
#include "quarkresources.h"
#include "quarkresourceloader.h"

#define IS_DELIMITER(c) (c == '\\' || c == '/')

std::vector<std::string> parseResourceIndex(const char* filePath){
    std::string filePathStr = filePath;

    std::vector<std::string> resourceIndex;

    std::ifstream resourceIndexFile(filePathStr + "/resources.txt");
    std::string line;
    if(resourceIndexFile.is_open()){
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Loading resources index...");
        while(std::getline(resourceIndexFile, line)){
            resourceIndex.push_back(line);
        }
        resourceIndexFile.close();
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load resource index.");
    }
    return resourceIndex;
}

std::string QuarkResourceManager::getKey(std::string relativeFilePath)
{
    int extLength = relativeFilePath.size() - relativeFilePath.find_last_of('.');
    relativeFilePath = relativeFilePath.substr(0, relativeFilePath.size() - extLength);

    for(int i = 0; i < relativeFilePath.length(); ++i){
        char c = relativeFilePath[i];
        if(IS_DELIMITER(c)){
            relativeFilePath[i] = '.';
        }
    }
    return relativeFilePath;
}

QuarkResource* QuarkResourceManager::loadResource(std::string absoluteFilePath)
{
    int extLength = absoluteFilePath.length() - absoluteFilePath.find_last_of('.') - 1;
    std::string ext = absoluteFilePath.substr(absoluteFilePath.find_last_of('.')+1, extLength);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Loading %s", absoluteFilePath.c_str());

    QuarkResourceLoader* loader = loaders[ext];
    if(loader == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "No loader registered for extension: '%s'", ext.c_str());
    }

    QuarkResource* resource = loader->loadResource(absoluteFilePath.c_str());
    if(resource == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load resource: '%s'", absoluteFilePath.c_str());
    }
    return resource;
}

QuarkResourceManager::QuarkResourceManager()
{
    imgLoader.reset(new QuarkIMGLoader());
    loaders["png"] = imgLoader.get();
    loaders["bmp"] = imgLoader.get();
    loaders["jpeg"] = imgLoader.get();
    loaders["jpg"] = imgLoader.get();

    soundLoader.reset(new QuarkSoundLoader());
    loaders["wav"] = soundLoader.get();

    musLoader.reset(new QuarkMusicLoader());
    loaders["mod"] = musLoader.get();
    loaders["ogg"] = musLoader.get();
    loaders["flag"] = musLoader.get();
    loaders["mp3"] = musLoader.get();
    loaders["midi"] = musLoader.get();
    loaders["mid"] = musLoader.get();

    mappingLoader.reset(new QuarkJoyMappingsLoader());
    loaders["map"] = mappingLoader.get();
}

int QuarkResourceManager::loadResources(const char* resourcesPath)
{
    std::string resourcesPathStr(resourcesPath);
    std::vector<std::string> resourceIndex = parseResourceIndex(resourcesPath);
    for(auto it = std::begin(resourceIndex); it != std::end(resourceIndex); ++it){
        std::string relativePath = (*it);
        std::string filePath = resourcesPathStr +"/" + relativePath;
        std::string key = getKey(relativePath);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "'%s' ===> '%s'", filePath.c_str(), key.c_str());
        resourceMap[key] = std::unique_ptr<QuarkResource>(loadResource(filePath));
    }
    return 0;
}

QuarkResource *QuarkResourceManager::getResource(const char* key)
{
    return resourceMap[std::string(key)].get();
}

QuarkResourceManager::~QuarkResourceManager()
{

}

