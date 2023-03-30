#pragma once

#include "../Shape/Interface.h"
#include "../../math/color.h"
#include <type_traits>

namespace Engine {

struct Object final
{
    class ShapePtr
    {
        Shape::Interface* ptr_ = nullptr;
        public:
        ShapePtr() = default;
        explicit ShapePtr(const Shape::Interface& shape)
        { ptr_ = shape.Clone().release(); }

        ShapePtr(const ShapePtr& sptr) { if (sptr.ptr_) ptr_ = sptr.ptr_ -> Clone().release(); }
        ShapePtr(ShapePtr&& sptr) { ptr_ = sptr.ptr_; sptr.ptr_ = nullptr; }
        
        ShapePtr& operator=(const ShapePtr& sptr)
        {
            if (&sptr == this) return *this;
            delete ptr_;
            if (sptr.ptr_)
                ptr_ = sptr.ptr_ -> Clone().release();
            else
                ptr_ = nullptr;
            return *this;
        }
        ShapePtr& operator=(ShapePtr&& sptr)
        {
            if (&sptr == this) return *this; 
            ptr_ = sptr.ptr_;
            sptr.ptr_ = nullptr;
            return *this;
        }

        ~ShapePtr() { delete ptr_; }

                Shape::Interface& operator*()       { return *ptr_; }
        const   Shape::Interface& operator*() const { return *ptr_; }
                Shape::Interface* operator->()          { return ptr_; }
        const   Shape::Interface* operator->() const    { return ptr_; }
    };

    ShapePtr shape_p;
    Color color = Color(1, 1, 1);
    float albedo[2] = {1, 0};
    float specExp = 100;
};

} // namespace Engine
