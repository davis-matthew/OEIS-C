/*
 * A370731  a(n) is the least k >= 0 such that F(k) AND n = n (where F(k)
 *          denotes A000045(k), the k-th Fibonacci number and AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A370731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k >= 0 such that F(k) AND n = n, where F(k) is the
 *            k-th Fibonacci number, AND is bitwise AND
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* NOTE: idx_t is uint64_t and seq_t is an unsigned integer type of
   arbitrary width (uint8_t ... uint128_t or an arbitrary‑precision
   type). All arithmetic here fits in 128 bits, so no overflow
   occurs. */

seq_t a(idx_t n)
{
    if (n == 0)                /* F(0) = 0 satisfies the condition */
        return (seq_t)0;

    /* Compute bit length of n (M) */
    unsigned M = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++M;
    }

    /* Mask to keep only the lower M bits. For  M == 64 the mask is
       all ones. */
    uint64_t mask = (M == 64) ? ~0ULL : ((uint64_t)1 << M) - 1;

    /* Fibonacci values modulo 2^M, kept in 64‑bit variables. */
    uint64_t fib0 = 0;   /* F(0) */
    uint64_t fib1 = 1;   /* F(1) */
    idx_t k = 0;

    for (;;) {
        /* If current Fibonacci number has all bits of n set, return k. */
        if ((fib0 & n) == n)
            return (seq_t)k;

        /* Compute next Fibonacci number modulo 2^M. */
        unsigned __int128 sum = (unsigned __int128)fib0 + fib1;
        fib0 = fib1;
        fib1 = (uint64_t)(sum & mask);

        ++k;
    }
}
