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
#include <Box2D/Dynamics/Contacts/Contact.hpp>

using namespace box2d;

TEST(Contact, ByteSize)
{
	switch (sizeof(RealNum))
	{
		case  4: EXPECT_EQ(sizeof(Contact), size_t(112)); break;
		case  8: EXPECT_EQ(sizeof(Contact), size_t(192)); break;
		case 16: EXPECT_EQ(sizeof(Contact), size_t(448)); break;
		default: FAIL(); break;
	}
}

TEST(Contact, IsNotDefaultConstructible)
{
	EXPECT_FALSE(std::is_default_constructible<Contact>::value);
}

TEST(Contact, IsNotCopyConstructible)
{
	EXPECT_FALSE(std::is_copy_constructible<Contact>::value);
}

TEST(Contact, IsNotCopyAssignable)
{
	EXPECT_FALSE(std::is_copy_assignable<Contact>::value);	
}
