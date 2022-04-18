#pragma once

namespace mrko900::gravity::gl {
    class GLAccess {
    public:
        typedef void (*FuncPtr) ();
        virtual ~GLAccess();
        virtual bool loadFunction(const char* name) = 0;
        virtual bool unloadFunction(const char* name) = 0;
        virtual bool isFunctionLoaded(const char* name) const = 0;
        virtual FuncPtr getFunction(const char* name) const = 0;
        
        // todo function to retrieve all loaded functions
    };
}
