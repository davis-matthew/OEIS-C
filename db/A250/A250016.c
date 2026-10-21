/*
 * A250016  Number of length 2+5 0..n arrays with no six consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*(n^7 + 2*n^6 + 4*n^5 + 5*n^4) + 17*n^3 + 9*n^2 - 2*n)
 *            / 3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer recurrence: a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 
   56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)   for n>8, with a(1)=20,...,a(8)=2776080 */

/* This implementation computes the terms via the explicit polynomial,
   which requires only unsigned arithmetic and works for arbitrary‑precision
   seq_t as well as for 8/16/32/64‑bit unsigned integers. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* base cases for n = 1..8 */
    static const seq_t base[8] = {
        (seq_t)20,
        (seq_t)520,
        (seq_t)5200,
        (seq_t)30360,
        (seq_t)125780,
        (seq_t)412160,
        (seq_t)1140160,
        (seq_t)2776080
    };

    if (n <= 8) {
        return base[n - 1];
    }

    /* compute n^k iteratively to avoid multiple casts */
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;          /* n^2 */
    seq_t n3 = n2 * n0;          /* n^3 */
    seq_t n4 = n3 * n0;          /* n^4 */
    seq_t n5 = n4 * n0;          /* n^5 */
    seq_t n6 = n5 * n0;          /* n^6 */
    seq_t n7 = n6 * n0;          /* n^7 */

    /* compute numerator = 3*(n^7 + 2*n^6 + 4*n^5 + 5*n^4) + 17*n^3 + 9*n^2 - 2*n */
    seq_t t1 = n7;                     /* n^7 */
    seq_t t2 = 2 * n6;                 /* 2*n^6 */
    seq_t t3 = 4 * n5;                 /* 4*n^5 */
    seq_t t4 = 5 * n4;                 /* 5*n^4 */

    seq_t sum = t1 + t2 + t3 + t4;     /* n^7 + 2*n^6 + 4*n^5 + 5*n^4 */
    sum = 3 * sum;                     /* 3*(...) */

    seq_t t5 = 17 * n3;                /* 17*n^3 */
    seq_t t6 = 9 * n2;                 /* 9*n^2 */

    sum = sum + t5 + t6;               /* add remaining positive terms */

    seq_t t7 = 2 * n0;                 /* 2*n (to be subtracted) */

    seq_t numerator = sum - t7;        /* numerator is guaranteed >= 0 for n>=1 */

    return numerator / (seq_t)3;       /* exact integer division by 3 */
}
