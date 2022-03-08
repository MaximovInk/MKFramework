#pragma once
#include <string>
#include <map>

namespace MKEngine {
    class Resource
    {
    public:
        virtual void Load(const std::string& sourcePath);
        virtual void Unload();
    };

    class ResourceManager
    {
        std::map< std::string, Resource* > m_Resources;
    public:

        template< class T >
        T* GetResource(const std::string& sourcePath)
        {
            if (m_Resources[sourcePath] != 0)
            {
                return dynamic_cast<T*>(m_Resources[sourcePath]);
            }

            T* newT = new T();

            newT->Load(sourcePath);
            m_Resources[sourcePath] = newT;
            return newT;
        }
    };

    class ShaderResource: Resource {
    private:
        //shader shaderS;

    public:
        void Load(const std::string& sourcePath) override {

        }
        virtual void Unload() {

        }
    };

}