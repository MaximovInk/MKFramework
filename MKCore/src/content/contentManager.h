#pragma once
#include <string>
#include <map>
#include "MKUtils.h"
//#include "MKGRAPHICS.H"
#include "shader.h"
#include "texture.h"

#include "iostream"
#include "fstream"
#include "sstream"
#include <filesystem>


namespace MKEngine {

    std::string get_file_contents(const char* filename) {
        std::ifstream in(filename, std::ios::binary);

        LOG::info(filename);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        throw(errno);
    }

    bool file_exists(const std::string filename) {
        return std::filesystem::exists(filename);
    }

    std::time_t getTimeLastModified(const char* filename) {

        struct stat fileattrib;
        stat(filename, &fileattrib);

        return fileattrib.st_mtime;
    }

    class Resource
    {
    public:

        virtual void Load(const std::string& sourcePath) = 0;
        virtual void Unload() = 0;
        virtual void Reload() = 0;
        virtual bool FileChanged() = 0;
    };

    class TextureResource : public Resource {
    private:
        std::string srcPath;
        
        std::time_t lastModifiedTime;

        void updateModTime() {
            lastModifiedTime = getTimeLastModified(srcPath.c_str());
        }

    public: 
        texture* tex;
        void Load(const std::string& sourcePath) override {
            tex = new texture(sourcePath.c_str());
            updateModTime();
        }

        virtual void Unload() {
            tex->destroy();
            delete tex;
        }

        virtual void Reload() {
            Unload();
            Load(srcPath);
        }

        virtual bool FileChanged() {

            std::time_t fileTime = getTimeLastModified(srcPath.c_str());

            return fileTime != lastModifiedTime;
        }
    };

    class ShaderResource :public Resource {
    public: shader* sh;

    private:
       
        std::string srcPath;

        std::time_t lastModifiedTime1;
        std::time_t lastModifiedTime2;
        std::time_t lastModifiedTime3;

        std::string getFragPath() {
            return srcPath + ".frag";
        }
        std::string getVertPath() {
            return srcPath + ".vert";
        }
        std::string getGeomPath() {
            return srcPath + ".geom";
        }

        bool isGeom;

        void updateModTime() {
            lastModifiedTime1 = getTimeLastModified(getVertPath().c_str());
            lastModifiedTime2 = getTimeLastModified(getFragPath().c_str());
            lastModifiedTime3 = getTimeLastModified(getGeomPath().c_str());
        }

    public:
        void Load(const std::string& sourcePath) override {
            srcPath = sourcePath;

            auto fragFilePath = getFragPath();
            auto vertFilePath = getVertPath();
            auto geomFilePath = getGeomPath();

            auto fragCode = get_file_contents(fragFilePath.c_str());
            auto vertCode = get_file_contents(vertFilePath.c_str());

            if (file_exists(sourcePath + ".geom")) {
                isGeom = true;
                auto geomCode = get_file_contents(geomFilePath.c_str());
                sh = new shader(vertCode.c_str(), fragCode.c_str(), geomCode.c_str());
                return;
            }

            sh = new shader(vertCode.c_str(), fragCode.c_str());
            updateModTime();
        }

        virtual void Unload() {
            sh->destroy();
            delete sh;
        }

        virtual bool FileChanged() {
            bool isChanged = false;

            std::time_t file1Time = getTimeLastModified(getVertPath().c_str());
            isChanged |= file1Time != lastModifiedTime1;
            std::time_t file2Time = getTimeLastModified(getFragPath().c_str());
            isChanged |= file2Time != lastModifiedTime2;

            if (isGeom) {
                std::time_t file3Time = getTimeLastModified(getGeomPath().c_str());

                isChanged |= file3Time != lastModifiedTime3;
            }
            return isChanged;
        }

        virtual void Reload() {
            Unload();
            Load(srcPath);
        }
    };

    class ContentPipe {
    private:
        std::map< std::string, Resource* > m_Resources;
    public:
        std::string resourcesDir;

        template< class T >
        T* GetResource(const std::string& sourcePath)
        {
            auto srcPath = (resourcesDir + sourcePath);
            if (m_Resources[srcPath] != 0)
            {
                return dynamic_cast<T*>(m_Resources[srcPath]);
            }

            T* newT = new T();

            newT->Load(srcPath);
            m_Resources[srcPath] = newT;
            return newT;
        }

        void updateResources() {
            std::map<std::string, Resource*>::iterator it;


            for (it = m_Resources.begin(); it != m_Resources.end(); it++)
            {
                auto res = it->second;

                if (res->FileChanged()) {
                    res->Reload();
                }

            }
        }



    };
}