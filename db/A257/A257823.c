/*
 * A257823  Positive integers whose square is the sum of 59 consecutive
 *          squares.
 * https://oeis.org/A257823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A257823 – Positive integers whose square is the sum of 59 consecutive squares
 *
 *  The sequence satisfies the linear recurrence
 *     a(n) = 1060*a(n-4) - a(n-8) ,   n ≥ 9
 *  with initial values
 *     a(1)=413 , a(2)=531 , a(3)=8673 , a(4)=11269 ,
 *     a(5)=426511 , a(6)=554187 , a(7)=9192849 , a(8)=11944727 .
 *
 *  The implementation uses fast exponentiation of the companion
 *  matrix of the recurrence.  No dynamic allocation is required
 *  and the code compiles both as C (C11) and C++ (C++17).
 *
 *  Author: ChatGPT – Aug 2023
 */

#include <stdint.h>   /* for uintptr_t, size_t */
#include <stdlib.h>   /* for abort, NULL */

#define MAT_SIZE 8

/* Forward declarations (to keep the code tidy) */
static void mat_identity(seq_t out[MAT_SIZE][MAT_SIZE]);
static void mat_zero(seq_t out[MAT_SIZE][MAT_SIZE]);
static void mat_mult(const seq_t A[MAT_SIZE][MAT_SIZE],
                     const seq_t B[MAT_SIZE][MAT_SIZE],
                     seq_t R[MAT_SIZE][MAT_SIZE]);
static void mat_pow(const seq_t base[MAT_SIZE][MAT_SIZE],
                    idx_t exp,
                    seq_t result[MAT_SIZE][MAT_SIZE]);

/* ---------------------  main sequence function  --------------------- */
seq_t a(idx_t n)
{
    /* the first eight terms are hard‑coded */
    static const seq_t seed[MAT_SIZE] = {
        (seq_t)11944727,   /* a(8) */
        (seq_t)9192849,    /* a(7) */
        (seq_t)554187,     /* a(6) */
        (seq_t)426511,     /* a(5) */
        (seq_t)11269,      /* a(4) */
        (seq_t)8673,       /* a(3) */
        (seq_t)531,        /* a(2) */
        (seq_t)413         /* a(1) */
    };

    if (n == 0) {
        return (seq_t)0;          /* unused – sequence is defined for n≥1 */
    }
    if (n <= (idx_t)MAT_SIZE) {
        return seed[MAT_SIZE - n];
    }

    /* Companion matrix of the recurrence           */
    /*  a(n)   = 1060*a(n-4) - a(n-8)
     *  a(n-1) = a(n-1)
     *  ...
     *  a(n-7) = a(n-7)
     *
     *  First row of T:
     *      T[0][3] = 1060   (a(n-4) term)
     *      T[0][7] = -1     (a(n-8) term)
     *
     *  Remaining rows perform the shift.
     */

    seq_t T[MAT_SIZE][MAT_SIZE];
    mat_zero(T);
    /* first row */
    T[0][3] = (seq_t)1060;
    T[0][7] = (seq_t)-1;     /* for unsigned seq_t this is the two's complement representation */
    /* shift rows */
    for (int i = 1; i < MAT_SIZE; ++i) {
        T[i][i - 1] = (seq_t)1;
    }

    /* Compute T^(n-8) */
    seq_t P[MAT_SIZE][MAT_SIZE];
    mat_pow(T, n - (idx_t)MAT_SIZE, P);

    /* Multiply P by the initial vector v8 = [a8,a7,...,a1] (already stored in seed[]) */
    seq_t result = (seq_t)0;
    for (int j = 0; j < MAT_SIZE; ++j) {
        /* seed[j] corresponds to a(8 - j) */
        result += P[0][j] * seed[j];
    }

    return result;
}

/* ---------------------  helper functions  --------------------- */

/* Return the 8×8 identity matrix in out. */
static void mat_identity(seq_t out[MAT_SIZE][MAT_SIZE])
{
    mat_zero(out);
    for (int i = 0; i < MAT_SIZE; ++i) {
        out[i][i] = (seq_t)1;
    }
}

/* Set every element of out to zero. */
static void mat_zero(seq_t out[MAT_SIZE][MAT_SIZE])
{
    for (int i = 0; i < MAT_SIZE; ++i) {
        for (int j = 0; j < MAT_SIZE; ++j) {
            out[i][j] = (seq_t)0;
        }
    }
}

/* R = A * B  (matrix multiplication, 8×8).  All arithmetic is performed in seq_t. */
static void mat_mult(const seq_t A[MAT_SIZE][MAT_SIZE],
                     const seq_t B[MAT_SIZE][MAT_SIZE],
                     seq_t R[MAT_SIZE][MAT_SIZE])
{
    for (int i = 0; i < MAT_SIZE; ++i) {
        for (int j = 0; j < MAT_SIZE; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < MAT_SIZE; ++k) {
                sum += A[i][k] * B[k][j];
            }
            R[i][j] = sum;
        }
    }
}

/* Compute result = base^exp  (exponentiation by squaring). */
static void mat_pow(const seq_t base[MAT_SIZE][MAT_SIZE],
                    idx_t exp,
                    seq_t result[MAT_SIZE][MAT_SIZE])
{
    seq_t cur[MAT_SIZE][MAT_SIZE];
    seq_t temp[MAT_SIZE][MAT_SIZE];
    mat_identity(result);
    /* copy base into cur */
    for (int i = 0; i < MAT_SIZE; ++i) {
        for (int j = 0; j < MAT_SIZE; ++j) {
            cur[i][j] = base[i][j];
        }
    }

    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) {
            mat_mult(result, cur, temp);
            for (int i = 0; i < MAT_SIZE; ++i) {
                for (int j = 0; j < MAT_SIZE; ++j) {
                    result[i][j] = temp[i][j];
                }
            }
        }
        mat_mult(cur, cur, temp);
        for (int i = 0; i < MAT_SIZE; ++i) {
            for (int j = 0; j < MAT_SIZE; ++j) {
                cur[i][j] = temp[i][j];
            }
        }
        exp >>= (idx_t)1;
    }
}
