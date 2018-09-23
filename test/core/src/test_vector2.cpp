#include <gtest/gtest.h>

#include <vector2.hpp>
#include <transform2.hpp>
#include "random_gen.hpp"

#include <cmath>

static const int OP_TEST_ITER = 250000;

using rvi::Vector2;

//////////////////////////////////////////////////////////////////////////
// OPERATORS
//////////////////////////////////////////////////////////////////////////

TEST (VectorOperators, Add)
{
    float acc_x = 0;
    float acc_y = 0;

    Vector2 result(0, 0);
    for(int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();

        Vector2 vec(x, y);

        result += vec;

        acc_x += x;
        acc_y += y;
    }

    ASSERT_FLOAT_EQ(result.X, acc_x);
    ASSERT_FLOAT_EQ(result.Y, acc_y);
}

TEST (VectorOperators, Sub)
{
	float acc_x = 0;
    float acc_y = 0;

    Vector2 result(0, 0);
    for(int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();

        Vector2 vec(x, y);

        result -= vec;

        acc_x -= x;
        acc_y -= y;
    }
    
    ASSERT_FLOAT_EQ(result.X, acc_x);
    ASSERT_FLOAT_EQ(result.Y, acc_y);
}

TEST (VectorOperators, MultiplyScalar)
{
	float acc_x = 0;
    float acc_y = 0;

    Vector2 result(0, 0);
    for(int i = 0; i < OP_TEST_ITER; i++)
    {
        float mul = GetRandomFloat();

        result *= mul;

        acc_x *= mul;
        acc_y *= mul;
    }

    ASSERT_FLOAT_EQ(result.X, acc_x);
    ASSERT_FLOAT_EQ(result.Y, acc_y);
}

TEST (VectorOperators, DivideScalar)
{
    for(int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();        

        Vector2 vec(x, y);

        float scale = GetRandomFloat(true);

        vec /= scale;

        ASSERT_FLOAT_EQ(vec.X, (x / scale));
        ASSERT_FLOAT_EQ(vec.Y, (y / scale));
    }
}

TEST (VectorOperators, MultiplyCrossProduct)
{
    float acc_x = 0;
    float acc_y = 0;

    Vector2 result(0, 0);
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();

        Vector2 vec(x, y);

        result *= vec;

        acc_x *= x;
        acc_y *= y;
    }

    ASSERT_FLOAT_EQ(result.X, acc_x);
    ASSERT_FLOAT_EQ(result.Y, acc_y);
}

TEST (VectorOperators, Equality)
{
    for(int i = 0; i < OP_TEST_ITER; i++)
    {
        Vector2 a(GetRandomFloat(), GetRandomFloat());
        Vector2 b(a.X, a.Y);
        Vector2 c(GetRandomFloat(), GetRandomFloat());

        ASSERT_TRUE(a == b);
        ASSERT_FALSE(a == c);
    }
}

TEST (VectorOperators, Inequality)
{
    for(int i = 0; i < OP_TEST_ITER; i++)
    {
        Vector2 a(GetRandomFloat(), GetRandomFloat());
        Vector2 b(a.X, a.Y);
        Vector2 c(GetRandomFloat(), GetRandomFloat());

        ASSERT_FALSE(a != b);
        ASSERT_TRUE(a != c);
    }
}

//////////////////////////////////////////////////////////////////////////
// METHODS
//////////////////////////////////////////////////////////////////////////

TEST (VectorMethods, Magnitude)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        Vector2 vec(GetRandomFloat(), GetRandomFloat());        
        float manualMagnitude = std::sqrtf(std::powf(vec.X, 2) + std::powf(vec.Y, 2));
        ASSERT_FLOAT_EQ(manualMagnitude, vec.Magnitude());
    }
}

TEST (VectorMethods, Offset)
{    
    float acc_x = 0;
    float acc_y = 0;
    Vector2 result(0, 0);
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        acc_x += x;
        acc_y += y;
        Vector2 offs(x, y);       
        result = result.Offset(offs);
    }

    ASSERT_FLOAT_EQ(result.X, acc_x);
    ASSERT_FLOAT_EQ(result.Y, acc_y);
}

TEST (VectorMethods, OffsetInPlace)
{
    float acc_x = 0;
    float acc_y = 0;
    Vector2 result(0, 0);
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        acc_x += x;
        acc_y += y;
        Vector2 offs(x, y);
        result.OffsetInPlace(offs);
    }

    ASSERT_FLOAT_EQ(result.X, acc_x);
    ASSERT_FLOAT_EQ(result.Y, acc_y);
}

TEST (VectorMethods, Rotate)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float mag = GetRandomFloat();
        Vector2 vec(mag, 0);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(vec.X, vec.Y);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(vec.X, 0.0F);
        ASSERT_FLOAT_EQ(vec.Y, mag);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(-vec.X, vec.Y);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(vec.X, -mag);
        ASSERT_FLOAT_EQ(vec.Y, 0.0F);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(-vec.X, -vec.Y);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(vec.X, 0.0F);
        ASSERT_FLOAT_EQ(vec.Y, -mag);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(vec.X, -vec.Y);

        vec = vec.Rotate(45.0F);
        ASSERT_FLOAT_EQ(vec.X, mag);
        ASSERT_FLOAT_EQ(vec.Y, 0.0F);
    }
}

TEST (VectorMethods, RotateInPlace)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float mag = GetRandomFloat();
        Vector2 vec(mag, 0);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(vec.X, vec.Y);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(vec.X, 0.0F);
        ASSERT_FLOAT_EQ(vec.Y, mag);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(-vec.X, vec.Y);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(vec.X, -mag);
        ASSERT_FLOAT_EQ(vec.Y, 0.0F);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(-vec.X, -vec.Y);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(vec.X, 0.0F);
        ASSERT_FLOAT_EQ(vec.Y, -mag);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(vec.X, -vec.Y);

        vec.RotateInPlace(45.0F);
        ASSERT_FLOAT_EQ(vec.X, mag);
        ASSERT_FLOAT_EQ(vec.Y, 0.0F);
    }
}

TEST (VectorMethods, Reverse)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        vec = vec.Reverse();

        ASSERT_FLOAT_EQ(-x, vec.X);
        ASSERT_FLOAT_EQ(-y, vec.Y);
    }
}

TEST (VectorMethods, ReverseInPlace)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        vec.ReverseInPlace();

        ASSERT_FLOAT_EQ(-x, vec.X);
        ASSERT_FLOAT_EQ(-y, vec.Y);
    }
}

TEST (VectorMethods, Scale_Scalar)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        float scale = GetRandomFloat();

        vec = vec.Scale(scale);

        ASSERT_FLOAT_EQ(x * scale, vec.X);
        ASSERT_FLOAT_EQ(y * scale, vec.Y);
    }
}

TEST (VectorMethods, Scale_CrossProduct)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        float scale_x = GetRandomFloat();
        float scale_y = GetRandomFloat();
        Vector2 scale(scale_x, scale_y);

        vec = vec.Scale(scale);

        ASSERT_FLOAT_EQ(x * scale_x, vec.X);
        ASSERT_FLOAT_EQ(y * scale_y, vec.Y);
    }
}

TEST (VectorMethods, ScaleInPlace_Scalar)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        float scale = GetRandomFloat();

        vec.ScaleInPlace(scale);

        ASSERT_FLOAT_EQ(x * scale, vec.X);
        ASSERT_FLOAT_EQ(y * scale, vec.Y);
    }
}

TEST (VectorMethods, ScaleInPlace_CrossProduct)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        float scale_x = GetRandomFloat();
        float scale_y = GetRandomFloat();
        Vector2 scale(scale_x, scale_y);

        vec.ScaleInPlace(scale);

        ASSERT_FLOAT_EQ(x * scale_x, vec.X);
        ASSERT_FLOAT_EQ(y * scale_y, vec.Y);
    }
}

TEST (VectorMethods, Invert)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat();
        float y = GetRandomFloat();
        Vector2 vec(x, y);

        vec = vec.Invert();

        ASSERT_FLOAT_EQ((1.0F / x), vec.X);
        ASSERT_FLOAT_EQ((1.0F / y), vec.Y);
    }
}

TEST (VectorMethods, InvertInPlace)
{
    for (int i = 0; i < OP_TEST_ITER; i++)
    {
        float x = GetRandomFloat(true);
        float y = GetRandomFloat(true);
        Vector2 vec(x, y);

        vec.InvertInPlace();

        ASSERT_FLOAT_EQ((1.0F / x), vec.X);
        ASSERT_FLOAT_EQ((1.0F / y), vec.Y);
    }
}