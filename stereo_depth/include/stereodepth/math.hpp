/***************************************************************************
 *            math.hpp
 *
 *  Copyright  2021  Mirco De Marchi
 *
 ****************************************************************************/


/*! \file  math.hpp
 *  \brief Deep Learning Math functionalities.
 */

#include "type.hpp"

#include <cmath>
#include <functional>
#include <cassert>
#include <stdexcept>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>
#include <numeric>
#include <string>

#include <iostream>

#ifndef STEREODEPTH_MATH_HPP
#define STEREODEPTH_MATH_HPP

namespace stereodepth {

class Math 
{
public:
    struct Coord2d {
        SizeType row;
        SizeType col;
    };

    struct Coord3d {
        Coord3d(Coord2d c2d)
            : row{c2d.row}
            , col{c2d.col}
            , channel{0}
        {}

        Coord3d(SizeType r, SizeType c, SizeType chn)
            : row{r}
            , col{c}
            , channel{chn}
        {}

        SizeType row;
        SizeType col;
        SizeType channel;
    };

    struct Shape {
    public:
        Shape(std::vector<SizeType> values)
            : _shape{values}
        {}

        [[nodiscard]] SizeType size() const
        {
            return std::accumulate(_shape.begin(), _shape.end(),
                                   SizeType(1),
                                   std::multiplies<SizeType>());
        }

        operator std::vector<SizeType>() const { return _shape; }

        SizeType& operator[](SizeType idx) { return _shape[idx]; }
        [[nodiscard]] const SizeType& at(SizeType idx) const
        { return _shape.at(idx); }

    protected:
        std::vector<SizeType> _shape;
    };

    struct Shape2d : public Shape {
    public:
        static inline SizeType SIZE = 2;
        static inline SizeType HEIGHT_IDX = 0;
        static inline SizeType WIDTH_IDX = 1;

        Shape2d(SizeType h, SizeType w)
            : Shape{{h,w}}
        {}

        Shape2d(SizeType s)
            : Shape{{s,s}}
        {}

        [[nodiscard]] const SizeType& height() const
        { return _shape[HEIGHT_IDX]; }
        [[nodiscard]]       SizeType& height()
        { return _shape[HEIGHT_IDX]; }

        [[nodiscard]] const SizeType& width() const
        { return _shape[WIDTH_IDX]; }
        [[nodiscard]]       SizeType& width()
        { return _shape[WIDTH_IDX]; }
    };

    struct Shape3d : public Shape {
    public:
        static inline SizeType SIZE = 3;
        static inline SizeType HEIGHT_IDX = 0;
        static inline SizeType WIDTH_IDX = 1;
        static inline SizeType CHANNEL_IDX = 2;

        Shape3d(Shape2d s2d)
            : Shape({s2d.height(),s2d.width(),1})
        {}

        Shape3d(SizeType h, SizeType w=1, SizeType c=1)
            : Shape({h,w,c})
        {}

        [[nodiscard]] const SizeType& height() const
        { return _shape[HEIGHT_IDX]; }
        [[nodiscard]]       SizeType& height()
        { return _shape[HEIGHT_IDX]; }

        [[nodiscard]] const SizeType& width() const
        { return _shape[WIDTH_IDX]; }
        [[nodiscard]]       SizeType& width()
        { return _shape[WIDTH_IDX]; }

        [[nodiscard]] const SizeType& channels() const
        { return _shape[CHANNEL_IDX]; }
        [[nodiscard]]       SizeType& channels()
        { return _shape[CHANNEL_IDX]; }
    };

    /**
     * \brief Find the argument that point to the maximum value.
     * \tparam T Type of the input.
     * \param src    Source array.
     * \param length Length of the array.
     * \return SizeType The argmax index.
     */
    template <typename T>
    static SizeType argmax(const T* src, SizeType length) 
    {
        return static_cast<SizeType>(std::distance(src, 
            std::max_element(src, src + length)));
    }

    /**
     * \brief Cross Correlation 2D of a source 2D matrix and a squared kernel.
     * \tparam T        Type of each source and destination elements.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width.
     * \param k         The kernel matrix to use for convolution.
     * \param k_shape   The shape of the kernel: height, width.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* cross_correlation(
        T* dst, const T* src, Shape2d src_shape, const T* k, Shape2d k_shape,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return cross_correlation<T>(
            dst, src, Shape3d(src_shape), k, k_shape, s, p);
    }

    /**
     * \brief Sum of squared differences in 2D slice of a source 2D matrix and 
     * a squared kernel.
     * \tparam T        Type of each source and destination elements.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width.
     * \param k         The kernel matrix to use for convolution.
     * \param k_shape   The shape of the kernel: height, width.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* squared_diff(
        T* dst, const T* src, Shape2d src_shape, const T* k, Shape2d k_shape,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return squared_diff<T>(
            dst, src, Shape3d(src_shape), k, k_shape, s, p);
    }

    /**
     * \brief Sum of absolute differences in 2D slice of a source 2D matrix and 
     * a squared kernel.
     * \tparam T        Type of each source and destination elements.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width.
     * \param k         The kernel matrix to use for convolution.
     * \param k_shape   The shape of the kernel: height, width.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* absolute_diff(
        T* dst, const T* src, Shape2d src_shape, const T* k, Shape2d k_shape,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return absolute_diff<T>(
            dst, src, Shape3d(src_shape), k, k_shape, s, p);
    }

    /**
     * \brief Cross Correlation 2D of a 3D source matrix of cubic kernel.
     * \tparam T        Type of each source and destination elements.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution.
     * \param k_shape   The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* cross_correlation(
        T* dst, const T* src, Shape3d src_shape, const T* k, Shape2d k_shape,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return kernel_slide<T>(
            _cross_correlation_op<T>, dst, src, src_shape, 
            k, k_shape, k_shape, {0, 0}, s, p);
    }

    /**
     * \brief Sum of squared differences in 2D slice of a 3D source matrix of 
     * cubic kernel.
     * \tparam T        Type of each source and destination elements.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution.
     * \param k_shape   The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* squared_diff(
        T* dst, const T* src, Shape3d src_shape, const T* k, Shape2d k_shape,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return kernel_slide<T>(
            _squared_diff_op<T>, dst, src, src_shape, 
            k, k_shape, k_shape, {0, 0}, 
            s, p);
    }

    /**
     * \brief Sum of absolute differences in 2D slice of a 3D source matrix of 
     * cubic kernel.
     * \tparam T        Type of each source and destination elements.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution.
     * \param k_shape   The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* absolute_diff(
        T* dst, const T* src, Shape3d src_shape, const T* k, Shape2d k_shape,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return kernel_slide<T>(
            _absolute_diff_op<T>, dst, src, src_shape, 
            k, k_shape, k_shape, {0, 0}, 
            s, p);
    }

    /**
     * \brief Cross Correlation 2D of a source 2D matrix and a squared kernel.
     * \tparam T         Type of each source and destination elements.
     * \param dst        The destination matrix in which put the resulting
     *                   matrix.
     * \param src1       The source matrix on which calculate the convolution.
     * \param src1_shape The shape of the source 1 matrix: height, width, 
     *                   channels.
     * \param src2       The source matrix from which take the kernel matrix to 
     *                   use for convolution.
     * \param src2_shape The shape of the source 2 matrix: height, width, 
     *                   channels.
     * \param k_shape    The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param k_offset   The offset in rows and cols to use in src2 matrix to 
     *                   take the kernel. 
     * \param s          The stride amount: the number of cells that the kernel
     *                   will move. It is defined in 2d: the width is the amount
     *                   stride when moving from left to right, the height
     *                   from up to down.
     * \param p          The padding of the source matrix to include defined in
     *                   2d: the width is the amount padding introduced in right
     *                   and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* cross_correlation_offset(
        T* dst, 
        const T* src1, Shape2d src1_shape, 
        const T* src2, Shape2d src2_shape, 
        Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return cross_correlation_offset<T>(
            dst, src1, Shape3d(src1_shape), src2, src2_shape, 
            k_shape, k_offset, s, p);
    }

    /**
     * \brief Sum of squared differences in 2D slice of a source 2D matrix and 
     * a squared kernel.
     * \tparam T         Type of each source and destination elements.
     * \param dst        The destination matrix in which put the resulting
     *                   matrix.
     * \param src1       The source matrix on which calculate the convolution.
     * \param src1_shape The shape of the source 1 matrix: height, width, 
     *                   channels.
     * \param src2       The source matrix from which take the kernel matrix to 
     *                   use for convolution.
     * \param src2_shape The shape of the source 2 matrix: height, width, 
     *                   channels.
     * \param k_shape    The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param k_offset   The offset in rows and cols to use in src2 matrix to 
     *                   take the kernel. 
     * \param s          The stride amount: the number of cells that the kernel
     *                   will move. It is defined in 2d: the width is the amount
     *                   stride when moving from left to right, the height
     *                   from up to down.
     * \param p          The padding of the source matrix to include defined in
     *                   2d: the width is the amount padding introduced in right
     *                   and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* squared_diff_offset(
        T* dst, 
        const T* src1, Shape2d src1_shape, 
        const T* src2, Shape2d src2_shape, 
        Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return squared_diff_offset<T>(
            dst, src1, Shape3d(src1_shape), src2, src2_shape, 
            k_shape, k_offset, s, p);
    }

    /**
     * \brief Sum of absolute differences in 2D slice of a source 2D matrix and 
     * a squared kernel.
     * \tparam T         Type of each source and destination elements.
     * \param dst        The destination matrix in which put the resulting
     *                   matrix.
     * \param src1       The source matrix on which calculate the convolution.
     * \param src1_shape The shape of the source 1 matrix: height, width, 
     *                   channels.
     * \param src2       The source matrix from which take the kernel matrix to 
     *                   use for convolution.
     * \param src2_shape The shape of the source 2 matrix: height, width, 
     *                   channels.
     * \param k_shape    The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param k_offset   The offset in rows and cols to use in src2 matrix to 
     *                   take the kernel. 
     * \param s          The stride amount: the number of cells that the kernel
     *                   will move. It is defined in 2d: the width is the amount
     *                   stride when moving from left to right, the height
     *                   from up to down.
     * \param p          The padding of the source matrix to include defined in
     *                   2d: the width is the amount padding introduced in right
     *                   and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* absolute_diff_offset(
        T* dst, 
        const T* src1, Shape2d src1_shape, 
        const T* src2, Shape2d src2_shape, 
        Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return absolute_diff_offset<T>(
            dst, src1, Shape3d(src1_shape), src2, src2_shape, 
            k_shape, k_offset, s, p);
    }
    
    /**
     * \brief Cross Correlation 2D of a 3D source matrix of cubic kernel.
     * \tparam T         Type of each source and destination elements.
     * \param dst        The destination matrix in which put the resulting
     *                   matrix.
     * \param src1       The source matrix on which calculate the convolution.
     * \param src1_shape The shape of the source 1 matrix: height, width, 
     *                   channels.
     * \param src2       The source matrix from which take the kernel matrix to 
     *                   use for convolution.
     * \param src2_shape The shape of the source 2 matrix: height, width.
     * The third dimension is the same of the src1 matrix.
     * \param k_shape    The shape of the kernel: height, width.
     * The third dimension is the same of the src1 matrix.
     * \param k_offset   The offset in rows and cols to use in src2 matrix to 
     *                   take the kernel. 
     * \param s          The stride amount: the number of cells that the kernel
     *                   will move. It is defined in 2d: the width is the amount
     *                   stride when moving from left to right, the height
     *                   from up to down.
     * \param p          The padding of the source matrix to include defined in
     *                   2d: the width is the amount padding introduced in right
     *                   and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* cross_correlation_offset(
        T* dst, 
        const T* src1, Shape3d src1_shape, 
        const T* src2, Shape2d src2_shape, 
        Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return kernel_slide<T>(
            _cross_correlation_op<T>, dst, src1, src1_shape, src2, src2_shape, 
            k_shape, k_offset, s, p);
    }

     /**
     * \brief Sum of squared differences in 2D slice of a 3D source matrix 
     * of cubic kernel.
     * \tparam T         Type of each source and destination elements.
     * \param dst        The destination matrix in which put the resulting
     *                   matrix.
     * \param src1       The source matrix on which calculate the convolution.
     * \param src1_shape The shape of the source 1 matrix: height, width, 
     *                   channels.
     * \param src2       The source matrix from which take the kernel matrix to 
     *                   use for convolution.
     * \param src2_shape The shape of the source 2 matrix: height, width, 
     *                   channels.
     * \param k_shape    The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param k_offset   The offset in rows and cols to use in src2 matrix to 
     *                   take the kernel. 
     * \param s          The stride amount: the number of cells that the kernel
     *                   will move. It is defined in 2d: the width is the amount
     *                   stride when moving from left to right, the height
     *                   from up to down.
     * \param p          The padding of the source matrix to include defined in
     *                   2d: the width is the amount padding introduced in right
     *                   and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* squared_diff_offset(
        T* dst, 
        const T* src1, Shape3d src1_shape, 
        const T* src2, Shape2d src2_shape, 
        Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return kernel_slide<T>(
            _squared_diff_op<T>, dst, src1, src1_shape, src2, src2_shape, 
            k_shape, k_offset, s, p);
    }

    /**
     * \brief Sum of absolute differences in 2D slice of a 3D source matrix 
     * of cubic kernel.
     * \tparam T         Type of each source and destination elements.
     * \param dst        The destination matrix in which put the resulting
     *                   matrix.
     * \param src1       The source matrix on which calculate the convolution.
     * \param src1_shape The shape of the source 1 matrix: height, width, 
     *                   channels.
     * \param src2       The source matrix from which take the kernel matrix to 
     *                   use for convolution.
     * \param src2_shape The shape of the source 2 matrix: height, width, 
     *                   channels.
     * \param k_shape    The shape of the kernel: height, width.
     * The third dimension is the same of the src matrix.
     * \param k_offset   The offset in rows and cols to use in src2 matrix to 
     *                   take the kernel. 
     * \param s          The stride amount: the number of cells that the kernel
     *                   will move. It is defined in 2d: the width is the amount
     *                   stride when moving from left to right, the height
     *                   from up to down.
     * \param p          The padding of the source matrix to include defined in
     *                   2d: the width is the amount padding introduced in right
     *                   and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - width_k  + (2 * p)) / s) + 1
     *  height_dst = ((height_src - height_k + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* absolute_diff_offset(
        T* dst, 
        const T* src1, Shape3d src1_shape, 
        const T* src2, Shape2d src2_shape, 
        Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        return kernel_slide<T>(
            _absolute_diff_op<T>, dst, src1, src1_shape, src2, src2_shape, 
            k_shape, k_offset, s, p);
    }

    /**
     * \brief Kernel slicing on the source matrix.
     * \tparam T        Type of each source and destination elements.
     * \param k_to_src_operation The operation to perform at each overlapping
     * step between the source matrix and the kernel.
     * \param dst       The destination matrix in which put the resulting
     *                  matrix.
     * \param src       The source matrix on which calculate the operation
     *                  defined in k_to_src_operation.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution. Can be nullptr
     *                  if the kernel is not used in k_to_src_operation (see
     *                  average pooling and max pooling).
     * \param k_real_shape The shape of the matrix containing the kernel: 
     *                  height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_shape   The shape of the kernel: height, width.
     * \param k_offset  The offset in rows and cols to use in k matrix to 
     *                  take the kernel. 
     *                  The third dimension is the same of the src matrix.
     * \param s         The stride amount: the number of cells that the kernel
     *                  will move. It is defined in 2d: the width is the amount
     *                  stride when moving from left to right, the height
     *                  from up to down.
     * \param p         The padding of the source matrix to include defined in
     *                  2d: the width is the amount padding introduced in right
     *                  and left side, the height in up and down side.
     * \return The pointer to the destination matrix.
     *
     * The destination matrix will be of shape:
     *  width_dst  = ((width_src  - f + (2 * p)) / s) + 1
     *  height_dst = ((height_src - f + (2 * p)) / s) + 1
     */
    template <typename T>
    static T* kernel_slide(
        std::function<void(T*, Shape2d, Coord2d,
                           const T*, Shape3d,
                           const T*, Shape2d, Shape2d, Shape2d,
                           int64_t, int64_t)> k_to_src_operation,
        T* dst, const T* src, Shape3d src_shape,
        const T* k, Shape2d k_real_shape, Shape2d k_shape, Shape2d k_offset,
        Shape2d s = {1, 1}, Shape2d p = {0, 0})
    {
        s.width() = std::max(s.width(), SizeType(1));
        s.height() = std::max(s.height(), SizeType(1));
        auto width_dst = src_shape.width() == 0 ? 0 :
            ((src_shape.width() - k_shape.width() + 2 * p.width()) / s.width()) + 1;
        auto height_dst = src_shape.height() == 0 ? 0 :
            ((src_shape.height() - k_shape.height() + 2 * p.height()) / s.height()) + 1;
        for (SizeType row_dst = 0; row_dst < height_dst; ++row_dst)
        {
            for (SizeType col_dst = 0; col_dst < width_dst; ++col_dst)
            {
                auto col = (static_cast<int64_t>(col_dst * s.width())
                    - static_cast<int64_t>(p.width()))
                    * static_cast<int64_t>(src_shape.channels());
                auto row = static_cast<int64_t>(row_dst * s.height())
                    - static_cast<int64_t>(p.height());
                k_to_src_operation(
                    dst, {height_dst, width_dst}, {row_dst, col_dst},
                    src, src_shape, k, k_real_shape, k_shape, k_offset, 
                    row, col);
            }
        }
        return dst;
    }

private:
    /**
     * \brief Sum of multiplication between the kernel and the source matrix
     * for Convolution 3D.
     * \tparam T        Type of each source and destination elements.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution.
     * \param k_real_shape The shape of the matrix containing the kernel: 
     *                  height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_shape   The shape of the kernel: height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_offset  The x offset to apply to the kernel to manage the case 
     *                  the kernel is inside a larger matrix. 
     * \param col       The column in which the kernel is moved over the source
     *                  matrix.
     * \param row       The row in which the kernel is moved over the source
     *                  matrix.
     * \return The value of the convolution 2D between the kernel and the
     * source matrix in the current position.
     */
    template <typename T>
    static void _cross_correlation_op(
        T* dst, Shape2d dst_shape, Coord2d dst_coord,
        const T* src, Shape3d src_shape,
        const T* k, Shape2d k_real_shape, Shape2d k_shape, Shape2d k_offset,
        int64_t row, int64_t col)
    {
        auto k_size = k_shape.size() * src_shape.channels();
        auto k_step = k_shape.width() * src_shape.channels();
        auto k_real_step = k_real_shape.width() * src_shape.channels();
        auto src_step = src_shape.width() * src_shape.channels();
        T sum = 0;
        k += k_offset.height() * k_real_step 
            + k_offset.width() * src_shape.channels();
        for (SizeType k_i = 0; k_i < k_size; ++k_i)
        {
            auto row_k = k_i / k_step;
            auto col_k = k_i % k_step;
            auto row_src = row + static_cast<int64_t>(row_k);
            auto col_src = col + static_cast<int64_t>(col_k);
            if (col_src < 0 || row_src < 0 ||
                col_src >= static_cast<int64_t>(src_step) ||
                row_src >= static_cast<int64_t>(src_shape.height()))
            {
                continue; //< zero-padding.
            }
            sum += src[row_src * static_cast<int64_t>(src_step) + col_src] 
                * k[row_k * k_real_step + col_k];
        }
        dst[dst_coord.row * dst_shape.width() + dst_coord.col] = sum;
    }

    /**
     * \brief Sum of squared differences between the kernel and the source
     * matrix for Convolution 3D.
     * \tparam T        Type of each source and destination elements.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution.
     * \param k_real_shape The shape of the matrix containing the kernel: 
     *                  height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_shape   The shape of the kernel: height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_offset  The x offset to apply to the kernel to manage the case 
     *                  the kernel is inside a larger matrix. 
     * \param col       The column in which the kernel is moved over the source
     *                  matrix.
     * \param row       The row in which the kernel is moved over the source
     *                  matrix.
     * \return The value of the sum of squared differences between the kernel 
     * and the source matrix in the current position.
     */
    template <typename T>
    static void _squared_diff_op(
        T* dst, Shape2d dst_shape, Coord2d dst_coord,
        const T* src, Shape3d src_shape,
        const T* k, Shape2d k_real_shape, Shape2d k_shape, Shape2d k_offset,
        int64_t row, int64_t col)
    {
        auto k_size = k_shape.size() * src_shape.channels();
        auto k_step = k_shape.width() * src_shape.channels();
        auto k_real_step = k_real_shape.width() * src_shape.channels();
        auto src_step = src_shape.width() * src_shape.channels();
        T sum = 0;
        T src_val;
        k += k_offset.height() * k_real_step 
            + k_offset.width() * src_shape.channels();
        for (SizeType k_i = 0; k_i < k_size; ++k_i)
        {
            auto row_k = k_i / k_step;
            auto col_k = k_i % k_step;
            auto row_src = row + static_cast<int64_t>(row_k);
            auto col_src = col + static_cast<int64_t>(col_k);
            if (col_src < 0 || row_src < 0 ||
                col_src >= static_cast<int64_t>(src_step) ||
                row_src >= static_cast<int64_t>(src_shape.height()))
            {
                src_val = 0; //< zero-padding.
            }
            else 
            {
                src_val = src[row_src * static_cast<int64_t>(src_step)
                    + col_src];
            }
            auto diff = src_val - k[row_k * k_real_step + col_k];
            sum += diff * diff; //< squared.
        }
        dst[dst_coord.row * dst_shape.width() + dst_coord.col] = sum;
    }

    /**
     * \brief Sum of absolute differences between the kernel and the source
     * matrix for Convolution 3D.
     * \tparam T        Type of each source and destination elements.
     * \param src       The source matrix on which calculate the convolution.
     * \param src_shape The shape of the source matrix: height, width, channels.
     * \param k         The kernel matrix to use for convolution.
     * \param k_real_shape The shape of the matrix containing the kernel: 
     *                  height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_shape   The shape of the kernel: height, width.
     *                  The third dimension is the same of the src matrix.
     * \param k_offset  The x offset to apply to the kernel to manage the case 
     *                  the kernel is inside a larger matrix. 
     * \param col       The column in which the kernel is moved over the source
     *                  matrix.
     * \param row       The row in which the kernel is moved over the source
     *                  matrix.
     * \return The value of the sum of absolute differences between the kernel 
     * and the source matrix in the current position.
     */
    template <typename T>
    static void _absolute_diff_op(
        T* dst, Shape2d dst_shape, Coord2d dst_coord,
        const T* src, Shape3d src_shape,
        const T* k, Shape2d k_real_shape, Shape2d k_shape, Shape2d k_offset,
        int64_t row, int64_t col)
    {
        auto k_size = k_shape.size() * src_shape.channels();
        auto k_step = k_shape.width() * src_shape.channels();
        auto k_real_step = k_real_shape.width() * src_shape.channels();
        auto src_step = src_shape.width() * src_shape.channels();
        T sum = 0;
        T src_val;
        k += k_offset.height() * k_real_step 
            + k_offset.width() * src_shape.channels();
        for (SizeType k_i = 0; k_i < k_size; ++k_i)
        {
            auto row_k = k_i / k_step;
            auto col_k = k_i % k_step;
            auto row_src = row + static_cast<int64_t>(row_k);
            auto col_src = col + static_cast<int64_t>(col_k);
            if (col_src < 0 || row_src < 0 ||
                col_src >= static_cast<int64_t>(src_step) ||
                row_src >= static_cast<int64_t>(src_shape.height()))
            {
                src_val = 0; //< zero-padding.
            }
            else 
            {
                src_val = src[row_src * static_cast<int64_t>(src_step)
                    + col_src];
            }
            auto diff = src_val - k[row_k * k_real_step + col_k];
            sum += diff > 0 ? diff : -diff; //< absolute.
        }
        dst[dst_coord.row * dst_shape.width() + dst_coord.col] = sum;
    }
};

} // namespace

#endif // STEREODEPTH_MATH_HPP
