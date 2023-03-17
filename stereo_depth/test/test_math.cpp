/***************************************************************************
 *            test_math.cpp
 *
 *  Copyright  2021  Mirco De Marchi
 *
 ****************************************************************************/

/*
 *  This file is part of EdgeLearning.
 *
 *  EdgeLearning is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EdgeLearning is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EdgeLearning.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "test.hpp"
#include "stereodepth/math.hpp"

#include <vector>
#include <iostream>
#include <iomanip>
#include <random>

using namespace std;
using namespace stereodepth;

class TestMath {
public:
    using TestNumType = double;

    void test() {
        TEST_CALL(test_shape());
        TEST_CALL(test_argmax());
        TEST_CALL(test_cross_correlation_without_channels());
        TEST_CALL(test_cross_correlation_with_channels());
        TEST_CALL(test_squared_diff_without_channels());
        TEST_CALL(test_squared_diff_with_channels());
        TEST_CALL(test_absolute_diff_without_channels());
        TEST_CALL(test_absolute_diff_with_channels());
        TEST_CALL(test_cross_correlation_without_channels_offset());
        TEST_CALL(test_cross_correlation_with_channels_offset());
        TEST_CALL(test_squared_diff_without_channels_offset());
        TEST_CALL(test_squared_diff_with_channels_offset());
        TEST_CALL(test_absolute_diff_without_channels_offset());
        TEST_CALL(test_absolute_diff_with_channels_offset());
    }

private:

    void test_shape() {
        SizeType h = 10;
        SizeType w = 11;
        Math::Shape2d shape_2d(h, w);
        TEST_EQUAL(shape_2d.height(), h);
        TEST_EQUAL(shape_2d.width(), w);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_2d).size(), 2);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_2d)[0], h);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_2d)[1], w);
        TEST_EQUAL(shape_2d[0], h);
        TEST_EQUAL(shape_2d[1], w);
        TEST_FAIL((void) shape_2d.at(2));
        TEST_THROWS((void) shape_2d.at(2), std::out_of_range);

        h = 12;
        w = 13;
        SizeType c = 3;
        Math::Shape3d shape_3d(h, w, c);
        TEST_EQUAL(shape_3d.height(), h);
        TEST_EQUAL(shape_3d.width(), w);
        TEST_EQUAL(shape_3d.channels(), c);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_3d).size(), 3);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_3d)[0], h);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_3d)[1], w);
        TEST_EQUAL(
                static_cast<std::vector<SizeType>>(shape_3d)[2], c);
        TEST_EQUAL(shape_3d[0], h);
        TEST_EQUAL(shape_3d[1], w);
        TEST_EQUAL(shape_3d[2], c);
        TEST_FAIL((void) shape_3d.at(3));
        TEST_THROWS((void) shape_3d.at(3), std::out_of_range);
    }

    void test_argmax() {
        std::vector<TestNumType> test_vec{0,1,5,4,3};
        TestNumType truth_argmax = 2;
        TestNumType ret_argmax = Math::argmax<TestNumType>(test_vec.data(), 
            test_vec.size());
        TEST_EQUAL(ret_argmax, truth_argmax);
    }

    void test_cross_correlation_without_channels() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8.5
        };
        std::vector<TestNumType> test_k{
            0, 0,
            0, 1
        };
        std::vector<TestNumType> truth_vec{
            4, 5,
            7, 8.5
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            0, 1, 2,   0,
            3, 4, 5,   0,
            6, 7, 8.5, 0,
            0, 0, 0,   0
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            0, 2,
            6, 8.5
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,  1,  2,  4,  5,
            3,  4,  5,  6,  7,
            6,  7,  8,  9,  10,
            9,  10, 11, 12, 13
        };
        test_k = std::vector<TestNumType>{
            0, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
        truth_vec = std::vector<TestNumType>{
            12, 14, 16,
            18, 20, 22
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            4,  6,  8,  11, 5,
            10, 12, 14, 16, 7,
            16, 18, 20, 22, 10,
            9,  10, 11, 12, 13
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            4,  8,  5,
            16, 20, 10,
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_cross_correlation_with_channels() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType input_channels = 2;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0,0, 1,1, 2,2,
            3,3, 4,4, 5,5,
            6,6, 7,7, 8.5,8.5
        };
        std::vector<TestNumType> test_k{
            0,0, 0,0,
            0,0, 1,1
        };
        std::vector<TestNumType> truth_vec{
            4+4, 5+5,
            7+7, 8.5+8.5
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            0+0, 1+1, 2+2,     0+0,
            3+3, 4+4, 5+5,     0+0,
            6+6, 7+7, 8.5+8.5, 0+0,
            0+0, 0+0, 0+0,     0+0
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            0+0, 2+2,
            6+6, 8.5+8.5
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        input_channels = 3;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,1,2,   4,5,0,   1,2,4,    5,0,1,   2,4,5,
            3,4,5,   6,7,3,   4,5,6,    7,3,4,   5,6,7,
            6,7,8,   9,10,6,  7,8,9,    10,6,7,  8,9,10,
            9,10,11, 12,13,9, 10,11,12, 13,9,10, 11,12,13
        };
        test_k = std::vector<TestNumType>{
            0,0,0, 0,0,0, 0,0,0,
            0,0,0, 1,1,1, 0,0,0,
            0,0,0, 0,0,0, 1,1,1
        };
        truth_vec = std::vector<TestNumType>{
            40, 38, 41,
            58, 56, 59
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            19, 24, 21, 24, 11,
            37, 40, 38, 41, 18,
            55, 58, 56, 59, 27,
            30, 34, 33, 32, 36
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            19, 21, 11,
            55, 56, 27
        };
        result.resize(truth_vec.size());
        Math::cross_correlation<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_squared_diff_without_channels() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8.5
        };
        std::vector<TestNumType> test_k{
            0, 0,
            0, 1
        };
        std::vector<TestNumType> truth_vec{
            19.0, 37.0, 
            97.0, 146.25,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            1.0,  0.0,  2.0,    5.0, 
            4.0,  19.0, 37.0,   30.0, 
            34.0, 97.0, 146.25, 98.25, 
            37.0, 86.0, 122.25, 73.25, 
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            1.0, 2.0, 
            34.0, 146.25,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,  1,  2,  4,  5,
            3,  4,  5,  6,  7,
            6,  7,  8,  9,  10,
            9,  10, 11, 12, 13
        };
        test_k = std::vector<TestNumType>{
            0, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
        truth_vec = std::vector<TestNumType>{
            182.0, 266.0, 370.0, 
            467.0, 598.0, 747.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            20.0, 45.0, 84.0, 135.0, 118.0, 
            93.0, 182.0, 266.0, 370.0, 295.0, 
            261.0, 467.0, 598.0, 747.0, 561.0, 
            250.0, 433.0, 539.0, 657.0, 470.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            20.0, 84.0, 118.0, 
        261.0, 598.0, 561.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_squared_diff_with_channels() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType input_channels = 2;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0,0, 1,1, 2,2,
            3,3, 4,4, 5,5,
            6,6, 7,7, 8.5,8.5
        };
        std::vector<TestNumType> test_k{
            0,0, 0,0,
            0,0, 1,1
        };
        std::vector<TestNumType> truth_vec{
            38.0, 74.0, 
            194.0, 292.5,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            2.0, 0.0, 4.0, 10.0, 
            8.0, 38.0, 74.0, 60.0, 
            68.0, 194.0, 292.5, 196.5, 
            74.0, 172.0, 244.5, 146.5,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            2.0, 4.0, 
            68.0, 292.5,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        input_channels = 3;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,1,2,   4,5,0,   1,2,4,    5,0,1,   2,4,5,
            3,4,5,   6,7,3,   4,5,6,    7,3,4,   5,6,7,
            6,7,8,   9,10,6,  7,8,9,    10,6,7,  8,9,10,
            9,10,11, 12,13,9, 10,11,12, 13,9,10, 11,12,13
        };
        test_k = std::vector<TestNumType>{
            0,0,0, 0,0,0, 0,0,0,
            0,0,0, 1,1,1, 0,0,0,
            0,0,0, 0,0,0, 1,1,1
        };
        truth_vec = std::vector<TestNumType>{
            774.0, 859.0, 901.0, 
            1732.0, 1844.0, 1922.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            158.0, 246.0, 297.0, 311.0, 239.0, 
            488.0, 774.0, 859.0, 901.0, 655.0, 
            1102.0, 1732.0, 1844.0, 1922.0, 1350.0, 
            1008.0, 1559.0, 1645.0, 1715.0, 1148.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            158.0, 297.0, 239.0, 
            1102.0, 1844.0, 1350.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_absolute_diff_without_channels() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8.5
        };
        std::vector<TestNumType> test_k{
            0, 0,
            0, 1
        };
        std::vector<TestNumType> truth_vec{
            7.0, 11.0, 
            19.0, 23.5,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            1.0, 0.0, 2.0, 3.0, 
            2.0, 7.0, 11.0, 8.0, 
            8.0, 19.0, 23.5, 14.5, 
            7.0, 14.0, 16.5, 9.5,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            1.0, 2.0, 
            8.0, 23.5,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,  1,  2,  4,  5,
            3,  4,  5,  6,  7,
            6,  7,  8,  9,  10,
            9,  10, 11, 12, 13
        };
        test_k = std::vector<TestNumType>{
            0, 0, 0,
            0, 1, 0,
            0, 0, 1
        };
        truth_vec = std::vector<TestNumType>{
            34.0, 44.0, 54.0, 
            61.0, 70.0, 79.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            8.0, 13.0, 20.0, 27.0, 22.0, 
            19.0, 34.0, 44.0, 54.0, 41.0, 
            37.0, 61.0, 70.0, 79.0, 57.0, 
            32.0, 51.0, 57.0, 63.0, 44.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            8.0, 20.0, 22.0, 
            37.0, 70.0, 57.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_absolute_diff_with_channels() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType input_channels = 2;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0,0, 1,1, 2,2,
            3,3, 4,4, 5,5,
            6,6, 7,7, 8.5,8.5
        };
        std::vector<TestNumType> test_k{
            0,0, 0,0,
            0,0, 1,1
        };
        std::vector<TestNumType> truth_vec{
            14.0, 22.0, 
            38.0, 47.0,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            2.0, 0.0, 4.0, 6.0, 
            4.0, 14.0, 22.0, 16.0, 
            16.0, 38.0, 47.0, 29.0, 
            14.0, 28.0, 33.0, 19.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            2.0, 4.0, 
            16.0, 47.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        input_channels = 3;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,1,2,   4,5,0,   1,2,4,    5,0,1,   2,4,5,
            3,4,5,   6,7,3,   4,5,6,    7,3,4,   5,6,7,
            6,7,8,   9,10,6,  7,8,9,    10,6,7,  8,9,10,
            9,10,11, 12,13,9, 10,11,12, 13,9,10, 11,12,13
        };
        test_k = std::vector<TestNumType>{
            0,0,0, 0,0,0, 0,0,0,
            0,0,0, 1,1,1, 0,0,0,
            0,0,0, 0,0,0, 1,1,1
        };
        truth_vec = std::vector<TestNumType>{
            126.0, 133.0, 139.0, 
            204.0, 210.0, 216.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            36.0, 58.0, 61.0, 67.0, 49.0, 
            80.0, 126.0, 133.0, 139.0, 99.0, 
            132.0, 204.0, 210.0, 216.0, 150.0, 
            110.0, 167.0, 171.0, 175.0, 118.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            36.0, 61.0, 49.0, 
            132.0, 210.0, 150.0
        };
        result.resize(truth_vec.size());
        Math::absolute_diff<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {f, f}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_cross_correlation_without_channels_offset() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8.5
        };
        std::vector<TestNumType> test_k{
            -1, -1, -1,
            -1,  0,  0,
            -1,  0,  1
        };
        std::vector<TestNumType> truth_vec{
            4, 5,
            7, 8.5
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
        
        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            0, 1, 2,   0,
            3, 4, 5,   0,
            6, 7, 8.5, 0,
            0, 0, 0,   0
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            0, 2,
            6, 8.5
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,  1,  2,  4,  5,
            3,  4,  5,  6,  7,
            6,  7,  8,  9,  10,
            9,  10, 11, 12, 13
        };
        test_k = std::vector<TestNumType>{
            -1, -1, -1, -1, -1, -1,
            -1, -1,  0,  0,  0, -1,
            -1, -1,  0,  1,  0, -1,
            -1, -1,  0,  0,  1, -1,
            -1, -1, -1, -1, -1, -1, 
        };
        truth_vec = std::vector<TestNumType>{
            12, 14, 16,
            18, 20, 22
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
        
        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            4,  6,  8,  11, 5,
            10, 12, 14, 16, 7,
            16, 18, 20, 22, 10,
            9,  10, 11, 12, 13
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            4,  8,  5,
            16, 20, 10,
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_cross_correlation_with_channels_offset() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType input_channels = 2;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0,0, 1,1, 2,2,
            3,3, 4,4, 5,5,
            6,6, 7,7, 8.5,8.5
        };
        std::vector<TestNumType> test_k{
            -1,-1, -1,-1, -1,-1,
            -1,-1,  0, 0,  0, 0,
            -1,-1,  0, 0,  1, 1
        };
        std::vector<TestNumType> truth_vec{
            4+4, 5+5,
            7+7, 8.5+8.5
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            0+0, 1+1, 2+2,     0+0,
            3+3, 4+4, 5+5,     0+0,
            6+6, 7+7, 8.5+8.5, 0+0,
            0+0, 0+0, 0+0,     0+0
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3,3}, {f, f}, {1, 1}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            0+0, 2+2,
            6+6, 8.5+8.5
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        input_channels = 3;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,1,2,   4,5,0,   1,2,4,    5,0,1,   2,4,5,
            3,4,5,   6,7,3,   4,5,6,    7,3,4,   5,6,7,
            6,7,8,   9,10,6,  7,8,9,    10,6,7,  8,9,10,
            9,10,11, 12,13,9, 10,11,12, 13,9,10, 11,12,13
        };
        test_k = std::vector<TestNumType>{
            -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  0, 0, 0,  0, 0, 0, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  1, 1, 1,  0, 0, 0, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  0, 0, 0,  1, 1, 1, -1,-1,-1,
            -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1,
        };
        truth_vec = std::vector<TestNumType>{
            40, 38, 41,
            58, 56, 59
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            19, 24, 21, 24, 11,
            37, 40, 38, 41, 18,
            55, 58, 56, 59, 27,
            30, 34, 33, 32, 36
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            19, 21, 11,
            55, 56, 27
        };
        result.resize(truth_vec.size());
        Math::cross_correlation_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_squared_diff_without_channels_offset() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8.5
        };
        std::vector<TestNumType> test_k{
            -1, -1, -1,
            -1,  0,  0,
            -1,  0,  1
        };
        std::vector<TestNumType> truth_vec{
            19.0, 37.0, 
            97.0, 146.25,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            1.0,  0.0,  2.0,    5.0, 
            4.0,  19.0, 37.0,   30.0, 
            34.0, 97.0, 146.25, 98.25, 
            37.0, 86.0, 122.25, 73.25, 
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            1.0, 2.0, 
            34.0, 146.25,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,  1,  2,  4,  5,
            3,  4,  5,  6,  7,
            6,  7,  8,  9,  10,
            9,  10, 11, 12, 13
        };
        test_k = std::vector<TestNumType>{
            -1, -1, -1, -1, -1, -1,
            -1, -1,  0,  0,  0, -1,
            -1, -1,  0,  1,  0, -1,
            -1, -1,  0,  0,  1, -1,
            -1, -1, -1, -1, -1, -1, 
        };
        truth_vec = std::vector<TestNumType>{
            182.0, 266.0, 370.0, 
            467.0, 598.0, 747.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            20.0, 45.0, 84.0, 135.0, 118.0, 
            93.0, 182.0, 266.0, 370.0, 295.0, 
            261.0, 467.0, 598.0, 747.0, 561.0, 
            250.0, 433.0, 539.0, 657.0, 470.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            20.0, 84.0, 118.0, 
        261.0, 598.0, 561.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_squared_diff_with_channels_offset() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType input_channels = 2;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0,0, 1,1, 2,2,
            3,3, 4,4, 5,5,
            6,6, 7,7, 8.5,8.5
        };
        std::vector<TestNumType> test_k{
            -1,-1, -1,-1, -1,-1,
            -1,-1,  0, 0,  0, 0,
            -1,-1,  0, 0,  1, 1
        };
        std::vector<TestNumType> truth_vec{
            38.0, 74.0, 
            194.0, 292.5,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            2.0, 0.0, 4.0, 10.0, 
            8.0, 38.0, 74.0, 60.0, 
            68.0, 194.0, 292.5, 196.5, 
            74.0, 172.0, 244.5, 146.5,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            2.0, 4.0, 
            68.0, 292.5,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        input_channels = 3;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,1,2,   4,5,0,   1,2,4,    5,0,1,   2,4,5,
            3,4,5,   6,7,3,   4,5,6,    7,3,4,   5,6,7,
            6,7,8,   9,10,6,  7,8,9,    10,6,7,  8,9,10,
            9,10,11, 12,13,9, 10,11,12, 13,9,10, 11,12,13
        };
        test_k = std::vector<TestNumType>{
            -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  0, 0, 0,  0, 0, 0, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  1, 1, 1,  0, 0, 0, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  0, 0, 0,  1, 1, 1, -1,-1,-1,
            -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1,
        };
        truth_vec = std::vector<TestNumType>{
            774.0, 859.0, 901.0, 
            1732.0, 1844.0, 1922.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            158.0, 246.0, 297.0, 311.0, 239.0, 
            488.0, 774.0, 859.0, 901.0, 655.0, 
            1102.0, 1732.0, 1844.0, 1922.0, 1350.0, 
            1008.0, 1559.0, 1645.0, 1715.0, 1148.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            158.0, 297.0, 239.0, 
            1102.0, 1844.0, 1350.0,
        };
        result.resize(truth_vec.size());
        Math::squared_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_absolute_diff_without_channels_offset() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0, 1, 2,
            3, 4, 5,
            6, 7, 8.5
        };
        std::vector<TestNumType> test_k{
            -1, -1, -1,
            -1,  0,  0,
            -1,  0,  1
        };
        std::vector<TestNumType> truth_vec{
            7.0, 11.0, 
            19.0, 23.5,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            1.0, 0.0, 2.0, 3.0, 
            2.0, 7.0, 11.0, 8.0, 
            8.0, 19.0, 23.5, 14.5, 
            7.0, 14.0, 16.5, 9.5,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            1.0, 2.0, 
            8.0, 23.5,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,  1,  2,  4,  5,
            3,  4,  5,  6,  7,
            6,  7,  8,  9,  10,
            9,  10, 11, 12, 13
        };
        test_k = std::vector<TestNumType>{
            -1, -1, -1, -1, -1, -1,
            -1, -1,  0,  0,  0, -1,
            -1, -1,  0,  1,  0, -1,
            -1, -1,  0,  0,  1, -1,
            -1, -1, -1, -1, -1, -1, 
        };
        truth_vec = std::vector<TestNumType>{
            34.0, 44.0, 54.0, 
            61.0, 70.0, 79.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            8.0, 13.0, 20.0, 27.0, 22.0, 
            19.0, 34.0, 44.0, 54.0, 41.0, 
            37.0, 61.0, 70.0, 79.0, 57.0, 
            32.0, 51.0, 57.0, 63.0, 44.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            8.0, 20.0, 22.0, 
            37.0, 70.0, 57.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            Math::Shape2d{input_height, input_width},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }

    void test_absolute_diff_with_channels_offset() {
        SizeType input_width = 3;
        SizeType input_height = 3;
        SizeType input_channels = 2;
        SizeType f = 2;
        SizeType output_width = 2;
        SizeType output_height = 2;
        std::vector<TestNumType> test_img{
            0,0, 1,1, 2,2,
            3,3, 4,4, 5,5,
            6,6, 7,7, 8.5,8.5
        };
        std::vector<TestNumType> test_k{
            -1,-1, -1,-1, -1,-1,
            -1,-1,  0, 0,  0, 0,
            -1,-1,  0, 0,  1, 1
        };
        std::vector<TestNumType> truth_vec{
            14.0, 22.0, 
            38.0, 47.0,
        };
        std::vector<TestNumType> result(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 4;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            2.0, 0.0, 4.0, 6.0, 
            4.0, 14.0, 22.0, 16.0, 
            16.0, 38.0, 47.0, 29.0, 
            14.0, 28.0, 33.0, 19.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 2;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            2.0, 4.0, 
            16.0, 47.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {3, 3}, {f, f}, {1, 1}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        input_width = 5;
        input_height = 4;
        input_channels = 3;
        f = 3;
        output_width = 3;
        output_height = 2;
        test_img = std::vector<TestNumType>{
            0,1,2,   4,5,0,   1,2,4,    5,0,1,   2,4,5,
            3,4,5,   6,7,3,   4,5,6,    7,3,4,   5,6,7,
            6,7,8,   9,10,6,  7,8,9,    10,6,7,  8,9,10,
            9,10,11, 12,13,9, 10,11,12, 13,9,10, 11,12,13
        };
        test_k = std::vector<TestNumType>{
            -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  0, 0, 0,  0, 0, 0, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  1, 1, 1,  0, 0, 0, -1,-1,-1,
            -1,-1,-1, -1,-1,-1,  0, 0, 0,  0, 0, 0,  1, 1, 1, -1,-1,-1,
            -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1, -1,-1,-1,
        };
        truth_vec = std::vector<TestNumType>{
            126.0, 133.0, 139.0, 
            204.0, 210.0, 216.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 5;
        output_height = 4;
        truth_vec = std::vector<TestNumType>{
            36.0, 58.0, 61.0, 67.0, 49.0, 
            80.0, 126.0, 133.0, 139.0, 99.0, 
            132.0, 204.0, 210.0, 216.0, 150.0, 
            110.0, 167.0, 171.0, 175.0, 118.0,
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {1, 1}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }

        output_width = 3;
        output_height = 2;
        truth_vec = std::vector<TestNumType>{
            36.0, 61.0, 49.0, 
            132.0, 210.0, 150.0
        };
        result.resize(truth_vec.size());
        Math::absolute_diff_offset<TestNumType>(
            result.data(), test_img.data(),
            {input_height, input_width, input_channels},
            test_k.data(), {5, 6}, {f, f}, {1, 2}, 
            {2, 2}, {1, 1});
        for (std::size_t r = 0; r < output_height; ++r)
        {
            for (std::size_t c = 0; c < output_width; ++c)
            {
                TEST_PRINT(
                    "[" + std::to_string(r) + "," + std::to_string(c) + "] "
                    + std::to_string(result[r * output_width + c]));
                TEST_WITHIN(
                    result[r * output_width + c],
                    truth_vec[r * output_width + c], 0.0000000000001);
            }
        }
    }
};

int main() {
    TestMath().test();
    return TEST_FAILURES;
}
