/*
 * Original work Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
 * Modified work Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/Box2D
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef Vec2_hpp
#define Vec2_hpp

#include <Box2D/Common/Settings.hpp>

namespace box2d
{
    /// Vector 2D.
    template <typename TYPE>
    struct Vector2D
    {
        using size_type = std::size_t;
        using data_type = TYPE;
        
        /// Default constructor does nothing (for performance).
        Vector2D() = default;
        
        Vector2D(const Vector2D& copy) = default;

        constexpr explicit Vector2D(data_type value) noexcept : x{value}, y{value} {}

        /// Construct using coordinates.
        constexpr Vector2D(data_type x_, data_type y_) noexcept : x{x_}, y{y_} {}
        
        /// Negate this vector.
        constexpr auto operator- () const noexcept { return Vector2D{-x, -y}; }
        
        /// Maximum size.
        /// @details This is this vector type's dimensionality.
        constexpr size_type max_size() const noexcept { return 2; }
        
        /// Accesses element by index.
        /// @param i Index (0 for x, 1 for y).
        auto operator[] (size_type i) const
        {
            assert(i < max_size());
            switch (i)
            {
                case 0: return x;
                case 1: return y;
                default: break;
            }
            return x;
        }
        
        /// Accesses element by index.
        /// @param i Index (0 for x, 1 for y).
        auto& operator[] (size_type i)
        {
            assert(i < max_size());
            switch (i)
            {
                case 0: return x;
                case 1: return y;
                default: break;
            }
            return x;
        }
        
        auto GetX() const noexcept
        {
            return x;
        }

        auto GetY() const noexcept
        {
            return y;
        }

        data_type x, y;
    };
#if 0
    template <typename TYPE>
    constexpr inline typename Vector2D<TYPE>::data_type GetX(const Vector2D<TYPE> value)
    {
        return value.x;
    }
    
    template <typename TYPE>
    constexpr inline typename Vector2D<TYPE>::data_type GetY(const Vector2D<TYPE> value)
    {
        return value.y;
    }
#endif
    template <typename TYPE>
    constexpr inline bool operator == (const Vector2D<TYPE> a, const Vector2D<TYPE> b) noexcept
    {
        return (a.x == b.x) && (a.y == b.y);
    }
    
    template <typename TYPE>
    constexpr inline bool operator != (const Vector2D<TYPE> a, const Vector2D<TYPE> b) noexcept
    {
        return (a.x != b.x) || (a.y != b.y);
    }

    /// Add two vectors component-wise.
    template <typename TYPE>
    constexpr inline Vector2D<TYPE> operator + (const Vector2D<TYPE> a, const Vector2D<TYPE> b) noexcept
    {
        return Vector2D<TYPE>{a.x + b.x, a.y + b.y};
    }
    
    /// Subtract two vectors component-wise.
    template <typename TYPE>
    constexpr inline Vector2D<TYPE> operator - (const Vector2D<TYPE> a, const Vector2D<TYPE> b) noexcept
    {
        return Vector2D<TYPE>{a.x - b.x, a.y - b.y};
    }
    
    /// Increment the left hand side value by the right hand side value.
    template <typename TYPE>
    constexpr Vector2D<TYPE>& operator += (Vector2D<TYPE>& lhs, const Vector2D<TYPE> rhs) noexcept
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }
    
    /// Decrement the left hand side value by the right hand side value.
    template <typename TYPE>
    constexpr Vector2D<TYPE>& operator -= (Vector2D<TYPE>& lhs, const Vector2D<TYPE> rhs) noexcept
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }
    
    template <typename TYPE>
    constexpr Vector2D<TYPE>& operator *= (Vector2D<TYPE>& lhs, const Real rhs) noexcept
    {
        lhs.x *= rhs;
        lhs.y *= rhs;
        return lhs;
    }
    
    template <typename TYPE>
    constexpr Vector2D<TYPE>& operator /= (Vector2D<TYPE>& lhs, const Real rhs) noexcept
    {
        lhs.x /= rhs;
        lhs.y /= rhs;
        return lhs;
    }
    
    template <typename TYPE1, typename TYPE2, typename OUT_TYPE = decltype(TYPE1{0} * TYPE2{0})>
    constexpr inline Vector2D<OUT_TYPE> operator * (const TYPE1 s, const Vector2D<TYPE2> a) noexcept
    {
        return Vector2D<OUT_TYPE>{s * a.x, s * a.y};
    }
    
    template <typename TYPE1, typename TYPE2, typename OUT_TYPE = decltype(TYPE1{0} * TYPE2{0})>
    constexpr inline Vector2D<OUT_TYPE> operator * (const Vector2D<TYPE1> a, const TYPE2 s) noexcept
    {
        return Vector2D<OUT_TYPE>{a.x * s, a.y * s};
    }
    
    template <typename TYPE1, typename TYPE2, typename OUT_TYPE = decltype(TYPE1{0} / TYPE2{0})>
    constexpr Vector2D<OUT_TYPE> operator/ (const Vector2D<TYPE1> a, const TYPE2 s) noexcept
    {
        return Vector2D<OUT_TYPE>{a.x / s, a.y / s};
    }
    
    /// @brief Vector 2D of Real.
    /// @note This data structure is two-times the size of the <code>Real</code> type
    ///   (or 8 using Real of float).
    using Vec2 = Vector2D<Real>;
    
    /// An all zero Vec2 value.
    /// @see Vec2.
    constexpr auto Vec2_zero = Vec2{0, 0};

    using Length2D = Vector2D<Length>;
    using LinearVelocity2D = Vector2D<LinearVelocity>;
    using LinearAcceleration2D = Vector2D<LinearAcceleration>;
    using Force2D = Vector2D<Force>;
    using Momentum2D = Vector2D<Momentum>;
        
    /// @brief Earthly gravity.
    /// @details An approximation of Earth's average gravity at sea-level.
    constexpr auto EarthlyGravity = LinearAcceleration2D{
        Real{0} * MeterPerSquareSecond,
        Real{-9.8f} * MeterPerSquareSecond
    };

    constexpr inline Vec2 GetVec2(const Vector2D<Real> value)
    {
        return value;
    }

    template <>
    constexpr inline Vec2 GetInvalid() noexcept
    {
        return Vec2{GetInvalid<Real>(), GetInvalid<Real>()};
    }

    /// Does this vector contain finite coordinates?
    template <typename TYPE>
    constexpr inline bool IsValid(const Vector2D<TYPE>& value) noexcept
    {
        return IsValid(value.x) && IsValid(value.y);
    }

#ifdef USE_BOOST_UNITS
    template <>
    constexpr Length2D GetInvalid() noexcept
    {
        return Length2D{GetInvalid<Length>(), GetInvalid<Length>()};
    }
    
    template <>
    constexpr LinearVelocity2D GetInvalid() noexcept
    {
        return LinearVelocity2D{GetInvalid<LinearVelocity>(), GetInvalid<LinearVelocity>()};
    }

    template <>
    constexpr Force2D GetInvalid() noexcept
    {
        return Force2D{GetInvalid<Force>(), GetInvalid<Force>()};
    }
    
    template <>
    constexpr Momentum2D GetInvalid() noexcept
    {
        return Momentum2D{GetInvalid<Momentum>(), GetInvalid<Momentum>()};
    }
    
    constexpr inline Vec2 GetVec2(const Length2D value)
    {
        return Vec2{value.x / Meter, value.y / Meter};
    }
    
    constexpr inline Vec2 GetVec2(const LinearVelocity2D value)
    {
        return Vec2{value.x / MeterPerSecond, value.y / MeterPerSecond};
    }
    
    constexpr inline Vec2 GetVec2(const Momentum2D value)
    {
        return Vec2{value.x / (Kilogram * MeterPerSecond), value.y / (Kilogram * MeterPerSecond)};
    }

    constexpr inline Vec2 GetVec2(const Force2D value)
    {
        return Vec2{value.x / Newton, value.y / Newton};
    }
#endif

} // namespace box2d

#endif /* Vec2_hpp */
