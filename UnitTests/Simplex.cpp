/*
 * Copyright (c) 2017 Louis Langholtz https://github.com/louis-langholtz/Box2D
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "gtest/gtest.h"
#include <Box2D/Collision/Simplex.hpp>

using namespace box2d;

TEST(SimplexCache, ByteSizeIs_12_16_or_32)
{
    switch (sizeof(Real))
    {
        case  4: EXPECT_EQ(sizeof(Simplex::Cache), std::size_t(12)); break;
        case  8: EXPECT_EQ(sizeof(Simplex::Cache), std::size_t(16)); break;
        case 16: EXPECT_EQ(sizeof(Simplex::Cache), std::size_t(32)); break;
        default: FAIL(); break;
    }
}

TEST(SimplexCache, IndexPairListByteSizeIs7)
{
    EXPECT_EQ(sizeof(Simplex::IndexPairs), std::size_t(7));
}

TEST(SimplexCache, DefaultInit)
{
    Simplex::Cache foo;
    EXPECT_EQ(decltype(foo.GetNumIndices()){0}, foo.GetNumIndices());
    EXPECT_FALSE(foo.IsMetricSet());
    
    // Can't test following cause of undefined behavior (assert's in debug build).
    //EXPECT_FALSE(IsValid(foo.GetMetric()));
}

TEST(SimplexCache, InitializingConstructor)
{
    {
        const auto metric = Real(.3);
        const auto indices = Simplex::IndexPairs{};
        Simplex::Cache foo{metric, indices};
        
        EXPECT_EQ(foo.GetNumIndices(), decltype(foo.GetNumIndices()){0});
        EXPECT_TRUE(foo.IsMetricSet());
        EXPECT_EQ(foo.GetMetric(), metric);
    }
    {
        const auto ip0 = IndexPair{0, 0};
        const auto ip1 = IndexPair{1, 0};
        const auto ip2 = IndexPair{4, 3};
        const auto metric = Real(-1.4);
        Simplex::Cache foo{metric, Simplex::IndexPairs{ip0, ip1, ip2}};
        
        EXPECT_EQ(foo.GetNumIndices(), decltype(foo.GetNumIndices()){3});
        EXPECT_EQ(foo.GetIndexPair(0), ip0);
        EXPECT_EQ(foo.GetIndexPair(1), ip1);
        EXPECT_EQ(foo.GetIndexPair(2), ip2);
        EXPECT_TRUE(foo.IsMetricSet());
        EXPECT_EQ(foo.GetMetric(), metric);
    }
}

TEST(SimplexCache, Assignment)
{
    const auto metric = Real(.3);
    const auto indices = Simplex::IndexPairs{};
    Simplex::Cache foo{metric, indices};
    
    ASSERT_EQ(foo.GetNumIndices(), decltype(foo.GetNumIndices()){0});
    ASSERT_TRUE(foo.IsMetricSet());
    ASSERT_EQ(foo.GetMetric(), metric);
    
    const auto ip0 = IndexPair{0, 0};
    const auto ip1 = IndexPair{1, 0};
    const auto ip2 = IndexPair{4, 3};
    const auto roo_metric = Real(-1.4);
    Simplex::Cache roo{roo_metric, Simplex::IndexPairs{ip0, ip1, ip2}};
    
    foo = roo;
    
    EXPECT_EQ(foo.GetNumIndices(), decltype(foo.GetNumIndices()){3});
    EXPECT_EQ(foo.GetIndexPair(0), ip0);
    EXPECT_EQ(foo.GetIndexPair(1), ip1);
    EXPECT_EQ(foo.GetIndexPair(2), ip2);
    EXPECT_TRUE(foo.IsMetricSet());
    EXPECT_EQ(foo.GetMetric(), roo_metric);
}

TEST(SimplexEdgeList, ByteSizeIs_88_176_or_352)
{
    switch (sizeof(Real))
    {
        case  4: EXPECT_EQ(sizeof(Simplex::Edges), std::size_t(88)); break;
        case  8: EXPECT_EQ(sizeof(Simplex::Edges), std::size_t(176)); break;
        case 16: EXPECT_EQ(sizeof(Simplex::Edges), std::size_t(352)); break;
        default: FAIL();
    }
}

TEST(Simplex, ByteSizeIs_104_208_or_416)
{
    switch (sizeof(Real))
    {
        case  4: EXPECT_EQ(sizeof(Simplex), std::size_t(104)); break;
        case  8: EXPECT_EQ(sizeof(Simplex), std::size_t(208)); break;
        case 16: EXPECT_EQ(sizeof(Simplex), std::size_t(416)); break;
        default: FAIL();
    }
}

TEST(Simplex, DefaultConstruction)
{
    Simplex foo;
    
    EXPECT_EQ(foo.GetSize(), decltype(foo.GetSize()){0});
}

TEST(Simplex, Get1)
{
    const auto va = Length2D{-Real(4) * Meter, Real(33) * Meter};
    const auto vb = Length2D{Real(901.5) * Meter, Real(0.06) * Meter};
    const auto ia = SimplexEdge::index_type{2};
    const auto ib = SimplexEdge::index_type{7};
    const auto sv = SimplexEdge{va, ia, vb, ib};
    
    const auto simplex = Simplex::Get(sv);
    EXPECT_EQ(simplex.GetSize(), decltype(simplex.GetSize()){1});

    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){0});
    const auto sv_new = simplex.GetSimplexEdge(0);
    EXPECT_EQ(sv.GetPointA(), sv_new.GetPointA());
    EXPECT_EQ(sv.GetIndexA(), sv_new.GetIndexA());
    EXPECT_EQ(sv.GetPointB(), sv_new.GetPointB());
    EXPECT_EQ(sv.GetIndexB(), sv_new.GetIndexB());
    
    const auto ce_new = simplex.GetCoefficient(0);
    EXPECT_EQ(ce_new, Real(1));
}

TEST(Simplex, Get2_of_same)
{
    const auto va = Length2D{-Real(4) * Meter, Real(33) * Meter};
    const auto vb = Length2D{Real(901.5) * Meter, Real(0.06) * Meter};
    const auto ia = SimplexEdge::index_type{2};
    const auto ib = SimplexEdge::index_type{7};
    const auto sv = SimplexEdge{va, ia, vb, ib};
    
    const auto simplex = Simplex::Get(sv, sv);
    EXPECT_EQ(simplex.GetSize(), decltype(simplex.GetSize()){1});
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){0});

    const auto sv_new = simplex.GetSimplexEdge(0);
    EXPECT_EQ(sv_new.GetPointA(), va);
    EXPECT_EQ(sv_new.GetIndexA(), ia);
    EXPECT_EQ(sv_new.GetPointB(), vb);
    EXPECT_EQ(sv_new.GetIndexB(), ib);
    
    const auto ce_new = simplex.GetCoefficient(0);
    EXPECT_EQ(ce_new, Real(1));
}

TEST(Simplex, Get2_fwd_perp)
{
    const auto va0 = Length2D{-Real(4) * Meter, Real(33) * Meter};
    const auto vb0 = Length2D{Real(901.5) * Meter, Real(0.06) * Meter};
    const auto ia0 = SimplexEdge::index_type{2};
    const auto ib0 = SimplexEdge::index_type{7};
    const auto sv0 = SimplexEdge{va0, ia0, vb0, ib0};

    const auto va1 = GetFwdPerpendicular(va0);
    const auto vb1 = GetFwdPerpendicular(vb0);
    const auto ia1 = SimplexEdge::index_type{4};
    const auto ib1 = SimplexEdge::index_type{1};
    const auto sv1 = SimplexEdge{va1, ia1, vb1, ib1};
    
    const auto simplex = Simplex::Get(sv0, sv1);
    EXPECT_EQ(simplex.GetSize(), decltype(simplex.GetSize()){2});
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){0});

    const auto sv_new_0 = simplex.GetSimplexEdge(0);
    EXPECT_EQ(sv_new_0.GetPointA(), va0);
    EXPECT_EQ(sv_new_0.GetIndexA(), ia0);
    EXPECT_EQ(sv_new_0.GetPointB(), vb0);
    EXPECT_EQ(sv_new_0.GetIndexB(), ib0);
    
    const auto ce_new_0 = simplex.GetCoefficient(0);
    EXPECT_TRUE(almost_equal(ce_new_0, Real(0.5)));
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){1});
    
    const auto sv_new_1 = simplex.GetSimplexEdge(1);
    EXPECT_EQ(sv_new_1.GetPointA(), va1);
    EXPECT_EQ(sv_new_1.GetIndexA(), ia1);
    EXPECT_EQ(sv_new_1.GetPointB(), vb1);
    EXPECT_EQ(sv_new_1.GetIndexB(), ib1);
    
    const auto ce_new_1 = simplex.GetCoefficient(1);
    EXPECT_TRUE(almost_equal(ce_new_1, Real(0.5)));
}

TEST(Simplex, Get2_rev_perp)
{
    const auto va0 = Length2D{-Real(4) * Meter, Real(33) * Meter};
    const auto vb0 = Length2D{Real(901.5) * Meter, Real(0.06) * Meter};
    const auto ia0 = SimplexEdge::index_type{2};
    const auto ib0 = SimplexEdge::index_type{7};
    const auto sv0 = SimplexEdge{va0, ia0, vb0, ib0};
    
    const auto va1 = GetRevPerpendicular(va0);
    const auto vb1 = GetRevPerpendicular(vb0);
    const auto ia1 = SimplexEdge::index_type{4};
    const auto ib1 = SimplexEdge::index_type{1};
    const auto sv1 = SimplexEdge{va1, ia1, vb1, ib1};
    
    const auto simplex = Simplex::Get(sv0, sv1);
    EXPECT_EQ(simplex.GetSize(), decltype(simplex.GetSize()){2});
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){0});
    
    const auto sv_new_0 = simplex.GetSimplexEdge(0);
    EXPECT_EQ(sv_new_0.GetPointA(), va0);
    EXPECT_EQ(sv_new_0.GetIndexA(), ia0);
    EXPECT_EQ(sv_new_0.GetPointB(), vb0);
    EXPECT_EQ(sv_new_0.GetIndexB(), ib0);
    
    const auto ce_new_0 = simplex.GetCoefficient(0);
    EXPECT_TRUE(almost_equal(ce_new_0, Real(0.5)));
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){1});
    
    const auto sv_new_1 = simplex.GetSimplexEdge(1);
    EXPECT_EQ(sv_new_1.GetPointA(), va1);
    EXPECT_EQ(sv_new_1.GetIndexA(), ia1);
    EXPECT_EQ(sv_new_1.GetPointB(), vb1);
    EXPECT_EQ(sv_new_1.GetIndexB(), ib1);
    
    const auto ce_new_1 = simplex.GetCoefficient(1);
    EXPECT_TRUE(almost_equal(ce_new_1, Real(0.5)));
}

TEST(Simplex, Get2_rot_plus_45)
{
    const auto va0 = Length2D{-Real(4) * Meter, Real(33) * Meter};
    const auto vb0 = Length2D{Real(901.5) * Meter, Real(0.06) * Meter};
    const auto ia0 = SimplexEdge::index_type{2};
    const auto ib0 = SimplexEdge::index_type{7};
    const auto sv0 = SimplexEdge{va0, ia0, vb0, ib0};
    
    const auto va1 = Rotate(va0, UnitVec2{Angle{Real{45.0f} * Degree}});
    const auto vb1 = Rotate(vb0, UnitVec2{Angle{Real{45.0f} * Degree}});
    const auto ia1 = SimplexEdge::index_type{4};
    const auto ib1 = SimplexEdge::index_type{1};
    const auto sv1 = SimplexEdge{va1, ia1, vb1, ib1};
    
    const auto simplex = Simplex::Get(sv0, sv1);
    EXPECT_EQ(simplex.GetSize(), decltype(simplex.GetSize()){2});
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){0});
    
    const auto sv_new_0 = simplex.GetSimplexEdge(0);
    EXPECT_EQ(sv_new_0.GetPointA(), va0);
    EXPECT_EQ(sv_new_0.GetIndexA(), ia0);
    EXPECT_EQ(sv_new_0.GetPointB(), vb0);
    EXPECT_EQ(sv_new_0.GetIndexB(), ib0);
    
    const auto ce_new_0 = simplex.GetCoefficient(0);
    EXPECT_TRUE(almost_equal(ce_new_0, Real(0.5)));
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){1});
    
    const auto sv_new_1 = simplex.GetSimplexEdge(1);
    EXPECT_EQ(sv_new_1.GetPointA(), va1);
    EXPECT_EQ(sv_new_1.GetIndexA(), ia1);
    EXPECT_EQ(sv_new_1.GetPointB(), vb1);
    EXPECT_EQ(sv_new_1.GetIndexB(), ib1);
    
    const auto ce_new_1 = simplex.GetCoefficient(1);
    EXPECT_TRUE(almost_equal(ce_new_1, Real(0.5)));
}

TEST(Simplex, Get2_rot45_half)
{
    const auto va0 = Length2D{-Real(4) * Meter, Real(33) * Meter}; // upper left
    const auto vb0 = Length2D{Real(901) * Meter, Real(6) * Meter}; // lower right
    const auto ia0 = SimplexEdge::index_type{2};
    const auto ib0 = SimplexEdge::index_type{7};
    const auto sv0 = SimplexEdge{va0, ia0, vb0, ib0};
    
    const auto va1 = Rotate(va0, UnitVec2{Angle{Real{45.0f} * Degree}}) / Real{2}; // Vec2{-13.081475, 10.253049}
    const auto vb1 = Rotate(vb0, UnitVec2{Angle{Real{45.0f} * Degree}}) / Real{2}; // Vec2{316.4303, 320.67291}
    EXPECT_NEAR(double(Real{va1.x / Meter}), -13.081475, 0.001);
    EXPECT_NEAR(double(Real{va1.y / Meter}),  10.253049, 0.001);
    EXPECT_NEAR(double(Real{vb1.x / Meter}), 316.4303,   0.001);
    EXPECT_NEAR(double(Real{vb1.y / Meter}), 320.67291,  0.001);
    const auto ia1 = SimplexEdge::index_type{4};
    const auto ib1 = SimplexEdge::index_type{1};
    const auto sv1 = SimplexEdge{va1, ia1, vb1, ib1};

    const auto w1 = vb0 - va0; // Vec2{901, 6} - Vec2{-4, 33} = Vec2{905, -27}
    EXPECT_TRUE(almost_equal(w1.x / Meter, Real(905)));
    EXPECT_TRUE(almost_equal(w1.y / Meter, Real(-27)));
    const auto w2 = vb1 - va1; // Vec2{316.4303, 320.67291} - Vec2{-13.081475, 10.253049} = Vec2{329.51178, 310.41986}
    EXPECT_NEAR(double(Real{w2.x / Meter}), 329.51178, 0.001);
    EXPECT_NEAR(double(Real{w2.y / Meter}), 310.41986, 0.001);
    
    const auto e12 = w2 - w1; // Vec2{329.51178, 310.41986} - Vec2{905, -27} = Vec2{-575.48822, 337.41986}
    EXPECT_NEAR(double(Real{e12.x / Meter}), -575.48822, 0.001);
    EXPECT_NEAR(double(Real{e12.y / Meter}),  337.41986, 0.001);

    const auto d12_2 = Area{-Dot(w1, e12)}; // -Dot(Vec2{905, -27}, Vec2{-575.48822, 337.41986}) = 529927.19
    EXPECT_NEAR(double(Real{d12_2 / SquareMeter}), 529927.19, 1.0);

    const auto d12_1 = Area{Dot(w2, e12)}; // Dot(Vec2{329.51178, 310.41986}, Vec2{-575.48822, 337.41986}) = -84888.312
    EXPECT_NEAR(double(Real{d12_1 / SquareMeter}), -84888.312, 1.0);

    const auto simplex = Simplex::Get(sv0, sv1);
    
    EXPECT_EQ(simplex.GetSize(), decltype(simplex.GetSize()){1});
    
    ASSERT_GT(simplex.GetSize(), decltype(simplex.GetSize()){0});
    
    const auto sv_new_0 = simplex.GetSimplexEdge(0);
    EXPECT_EQ(sv_new_0.GetPointA(), va1);
    EXPECT_EQ(sv_new_0.GetIndexA(), ia1);
    EXPECT_EQ(sv_new_0.GetPointB(), vb1);
    EXPECT_EQ(sv_new_0.GetIndexB(), ib1);
    
    const auto ce_new_0 = simplex.GetCoefficient(0);
    EXPECT_TRUE(almost_equal(ce_new_0, Real(1)));
}

TEST(Simplex, GetOfSimplexVertices)
{
    Simplex foo;

    const auto roo = Simplex::Get(foo.GetEdges());
    
    EXPECT_EQ(foo.GetSize(), roo.GetSize());
}
