/*
 * A344410  a(n) = (3*n^2 - 1) * (3*n^2 - 2) * (3*n^3 - 3*n + 1)/2.
 * https://oeis.org/A344410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 - 1) * (3*n^2 - 2) * (3*n^3 - 3*n + 1) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 389,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A344410, with a(0)=1, a(1)=1, … */
seq_t a(idx_t n)
{
    /* For n = 0 the factors 3*n^2-1 and 3*n^2-2 are negative.
       Since seq_t is unsigned, we return the known value. */
    if (n == 0)
        return (seq_t)1;

    /* Convert the index once to seq_t. */
    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;         /* n^2  */
    seq_t sn3 = sn2 * sn;        /* n^3  */

    /* Compute the three factors.  All are positive for n >= 1. */
    seq_t t1 = (seq_t)3 * sn2 - (seq_t)1;          /* 3*n^2 - 1 */
    seq_t t2 = (seq_t)3 * sn2 - (seq_t)2;          /* 3*n^2 - 2 */
    seq_t t3 = (seq_t)3 * sn3 - (seq_t)3 * sn + (seq_t)1; /* 3*n^3 - 3*n + 1 */

    /* One of t1 or t2 is even; divide that factor by 2 first to keep
       intermediate sizes small and preserve exactness. */
    if ((t1 & (seq_t)1) == 0)
        t1 = t1 / (seq_t)2;     /* t1 even */
    else
        t2 = t2 / (seq_t)2;     /* t2 even */

    /* Compute the product using seq_t arithmetic. */
    seq_t prod = t1;
    prod = prod * t2;
    prod = prod * t3;

    return prod;
}
