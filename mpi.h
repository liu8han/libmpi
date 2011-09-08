/**
 * \file mpi.h
 *
 * \brief  Multi-precision integer library
 *
 *  Copyright (C) 2011, Han Liu.
 *
 *  This file is modified from  PolarSSL for convient use to calculate Diffie-Hellman messages (http://www.polarssl.org)
 *  Lead Maintainer: Han Liu <cn.liuhan@gmail.com>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#define ERR_MPI_FILE_IO_ERROR                     -0x0002  /**< An error occurred while reading from or writing to a file. */
#define ERR_MPI_BAD_INPUT_DATA                    -0x0004  /**< Bad input parameters to function. */
#define ERR_MPI_INVALID_CHARACTER                 -0x0006  /**< There is an invalid character in the digit string. */
#define ERR_MPI_BUFFER_TOO_SMALL                  -0x0008  /**< The output buffer is too small to write too. */
#define ERR_MPI_NEGATIVE_VALUE                    -0x000A  /**< The input arguments are negative or result in illegal output. */
#define ERR_MPI_DIVISION_BY_ZERO                  -0x000C  /**< The input argument for division is zero, which is not allowed. */
#define ERR_MPI_NOT_ACCEPTABLE                    -0x000E  /**< The input arguments are not acceptable. */

typedef int32_t t_sint;
typedef uint32_t t_uint;

typedef struct
{
    int s; /*!<  integer sign      */
    size_t n; /*!<  total # of limbs  */
    t_uint *p; /*!<  pointer to limbs  */
}mpi;

#define MPI_MAX_LIMBS                             10000

/**
 * \brief           Initialize one MPI
 *
 * \param X         One MPI to initialize.
 */
void mpi_init (mpi *X);

/**
 * \brief          Unallocate one MPI
 *
 * \param X        One MPI to unallocate.
 */
void mpi_free (mpi *X);

/*
 * Enlarge total size to the specified # of limbs
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_grow (mpi *X, size_t nblimbs);

/*
 * Set value to integer z
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_lset (mpi *X, t_sint z);

/*
 * Copy the contents of Y into X
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_copy (mpi *X, const mpi *Y);


/*
 * Import unsigned value from binary data
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_import (mpi *X, const unsigned char *buf, size_t buflen);

/*
 * Export unsigned value into binary data
 *
 * Call this function with buflen = 0 to obtain the required
 * buffer size in buflen.
 *
 * Returns 0 if successful
 *         ERR_MPI_BUFFER_TOO_SMALL if buf hasn't enough room
 */
int mpi_export (const mpi *X, unsigned char *buf, size_t * buflen);

/**
 * \brief          Return the number of most significant bits
 *
 * \param X        MPI to use
 */
size_t mpi_msb (const mpi *X);


/*
 * Returns actual size in bits
 */
size_t mpi_size (const mpi *X);

/*
 * Left-shift: X <<= count
 *
 * Returns 0 if successful,
 *         1 if memory allocation failed
 */
int mpi_shift_l (mpi *X, size_t count);

/*
 * Right-shift: X >>= count
 *
 * Always returns 0.
 */
int mpi_shift_r (mpi *X, size_t count);

/*
 * Compare absolute values
 *
 * Returns 1 if |X| is greater than |Y|
 *        -1 if |X| is lesser  than |Y|
 *         0 if |X| is equal to |Y|
 */
int mpi_cmp_abs (const mpi *X, const mpi *Y);

/*
 * Compare signed values
 *
 * Returns 1 if X is greater than Y
 *        -1 if X is lesser  than Y
 *         0 if X is equal to Y
 */
int mpi_cmp_mpi (const mpi *X, const mpi *Y);

/*
 * Compare signed values
 *
 * Returns 1 if X is greater than z
 *        -1 if X is lesser  than z
 *         0 if X is equal to z
 */
int mpi_cmp_int (const mpi *X, t_sint z);

/*
 * Unsigned addition: X = |A| + |B|  (HAC 14.7)
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_add_abs (mpi *X, const mpi *A, const mpi *B);

/*
 * Unsigned substraction: X = |A| - |B|  (HAC 14.9)
 *
 * Returns 0 if successful
 *         ERR_MPI_NEGATIVE_VALUE if B is greater than A
 */
int mpi_sub_abs (mpi *X, const mpi *A, const mpi *B);

/*
 * Signed addition: X = A + B
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_add_mpi (mpi *X, const mpi *A, const mpi *B);

/*
 * Signed substraction: X = A - B
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_sub_mpi (mpi *X, const mpi *A, const mpi *B);

/**
 * \brief          Baseline multiplication: X = A * B
 *
 * \param X        Destination MPI
 * \param A        Left-hand MPI
 * \param B        Right-hand MPI
 *
 * \return         0 if successful,
 *                 1 if memory allocation failed
 */
int mpi_mul_mpi (mpi *X, const mpi *A, const mpi *B);

/*
 * Baseline multiplication: X = A * b
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 */
int mpi_mul_int (mpi *X, const mpi *A, t_sint b);

/**
 * \brief          Division by mpi: A = Q * B + R
 *
 * \param Q        Destination MPI for the quotient
 * \param R        Destination MPI for the rest value
 * \param A        Left-hand MPI
 * \param B        Right-hand MPI
 *
 * \return         0 if successful,
 *                 1 if memory allocation failed,
 *                 ERR_MPI_DIVISION_BY_ZERO if B == 0
 *
 * \note           Either Q or R can be NULL.
 */
int mpi_div_mpi (mpi *Q, mpi *R, const mpi *A, const mpi *B);

/**
 * \brief          Division by int: A = Q * b + R
 *
 * \param Q        Destination MPI for the quotient
 * \param R        Destination MPI for the rest value
 * \param A        Left-hand MPI
 * \param b        Integer to divide by
 *
 * \return         0 if successful,
 *                 1 if memory allocation failed,
 *                 ERR_MPI_DIVISION_BY_ZERO if b == 0
 *
 * \note           Either Q or R can be NULL.
 */
int mpi_div_int (mpi *Q, mpi *R, const mpi *A, t_sint b);

/**
 * \brief          Modulo: R = A mod B
 *
 * \param R        Destination MPI for the rest value
 * \param A        Left-hand MPI
 * \param B        Right-hand MPI
 *
 * \return         0 if successful,
 *                 1 if memory allocation failed,
 *                 ERR_MPI_DIVISION_BY_ZERO if B == 0,
 *                 ERR_MPI_NEGATIVE_VALUE if B < 0
 */
int mpi_mod_mpi (mpi *R, const mpi *A, const mpi *B);

/**
 * \brief          Modulo: r = A mod b
 *
 * \param r        Destination t_uint
 * \param A        Left-hand MPI
 * \param b        Integer to divide by
 *
 * \return         0 if successful,
 *                 1 if memory allocation failed,
 *                 ERR_MPI_DIVISION_BY_ZERO if b == 0,
 *                 ERR_MPI_NEGATIVE_VALUE if b < 0
 */
int mpi_mod_int (t_uint *r, const mpi *A, t_sint b);

/*
 * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
 *
 * Returns 0 if successful
 *         1 if memory allocation failed
 *         ERR_MPI_INVALID_PARAMETER if N is negative or even
 */
int mpi_exp_mod (mpi *X, const mpi *A, const mpi *E, const mpi *N, mpi *_RR);

