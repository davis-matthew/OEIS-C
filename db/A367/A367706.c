/*
 * A367706  Number of degree 5 vertices in the n-Menger sponge graph.
 * https://oeis.org/A367706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14/85)*20^n + (2/5)*8^n - (864/85)*3^n + 24
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the integer form:                  */
/* 85*a(n) = 14*20^n + 34*8^n - 864*3^n + 2040                   */
/* For large seq_t types it computes the 3 powers with a fast   */
/* exponentiation routine and divides by 85.  For small seq_t    */
/* (≤ 64‑bit) it uses the recurrence                                    */
/*   a(n) = (100*a(n-1) - 3*8^n + 288*3^n - 2280) / 5              */
/* which requires only 128‑bit intermediates.                       */

#include <stdint.h>

#ifndef SEQ_T
/*  seq_t and idx_t are supplied by the build.  */
#endif

/* Fast exponentiation for integer bases and unsigned integer exponents.
   The result type is seq_t so it works transparently for arbitrary
   precision or small integer builds. */
static inline seq_t pow_uint_seq(uint64_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1)
            result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0.  */
    if (n <= 1)
        return 0;

    /* Small seq_t (≤ 64‑bit) implementation:
       use 128‑bit intermediate to avoid overflow for all n that
       fit into 64 bits (which covers up to at least n = 19). */
    if (sizeof(seq_t) <= 8)
    {
        __int128 a_prev = 0;          /* a(1) = 0            */
        __int128 pow8  = 8;            /* 8^1                 */
        __int128 pow3  = 3;            /* 3^1                 */

        for (idx_t i = 2; i <= n; ++i)
        {
            pow8  *= 8;
            pow3  *= 3;
            /* 5*a(i) = 100*a(i-1) - 3*8^i + 288*3^i - 2280 */
            __int128 numerator = 100 * a_prev - 3 * pow8 + 288 * pow3 - (const __int128)2280;
            a_prev = numerator / 5;
        }
        return (seq_t)a_prev;
    }

    /* Arbitrary‑precision implementation. */
    seq_t p20 = pow_uint_seq(20, n);
    seq_t p8  = pow_uint_seq(8,  n);
    seq_t p3  = pow_uint_seq(3,  n);

    seq_t numerator = (seq_t)14 * p20
                     + (seq_t)34 * p8
                     - (seq_t)864 * p3
                     + (seq_t)2040;
    return numerator / (seq_t)85;
}
