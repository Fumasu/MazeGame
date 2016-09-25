#pragma once

#include "Context.hpp"

class Drawable
{
public:
    Drawable(Context& ctx) : mContext(ctx)
    {
    }
    virtual ~Drawable()
    {
    }
    
    virtual void Draw () =0;
    
    Context& GetContext()
    {
        return mContext;
    }
private:
    Context& mContext;
};