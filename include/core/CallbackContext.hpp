#ifndef CALLBACK_CONTEXT_HPP
#define CALLBACK_CONTEXT_HPP

#include <string>
#include <unordered_map>

// This struct manages every Object etc We want to access from our callback functions
// An instance of this can be set using glfwSetWindowUserPointer and retrieved from our callback by calling glfwGetWindowUserPointer
// It wraps the objects we want to make acessible as (void*) which then can be cast to the right type
struct CallbackContext {
    private:
        std::unordered_map<std::string, void*> resources;

    public:
        template<typename T>
        T* get(const std::string& key) const {
            auto it = resources.find(key);
            if(it != resources.end()) 
                return static_cast<T*>(it->second);
            return nullptr;
        }

        template<typename T>
        void set(const std::string& key, T* ptr) {
            resources[key] = static_cast<void*>(ptr);
        }
};

#endif // !CALLBACK_CONTEXT_HPP 
