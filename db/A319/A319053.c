/*
 * A319053  a(n) is the exponent of the largest power of 2 that appears in the
 *          factorization of the entries in the matrix {{3,1},{1,-1}}^n.
 * https://oeis.org/A319053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 + max{ v2(F(n-3)), v2(F(n)), v2(F(n+3)) } (ignore 0
 *            entries)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   ctz_u64_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ctz_u64_to_u32.h"

/* Simple max for unsigned */
static unsigned usmax(unsigned a, unsigned b) { return a > b ? a : b; }

/* Fast doubling Fibonacci modulo 2^64 */
static void fib_pair(idx_t n, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair(n >> 1, &a, &b);            /* (F(k), F(k+1)) */
    uint64_t c = a * ((b << 1) - a);     /* F(2k)  */
    uint64_t d = a * a + b * b;          /* F(2k+1)*/
    if (n & 1) {
        *fn  = d;
        *fn1 = c + d;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* Sequence entry */
seq_t a(idx_t n)
{
    uint64_t base = n - 1;      /* 2^(n-1) is common factor */
    unsigned maxv2 = 0;
    uint64_t val, dummy;

    /* v2(F(n+3)) */
    fib_pair(n + 3, &val, &dummy);
    maxv2 = usmax(maxv2, ctz_u64_to_u32(val));

    /* v2(F(n))   */
    fib_pair(n, &val, &dummy);
    maxv2 = usmax(maxv2, ctz_u64_to_u32(val));

    /* v2(F(n-3))  – ignore when n==3 (value 0) */
    if (n > 3) {
        fib_pair(n - 3, &val, &dummy);
        maxv2 = usmax(maxv2, ctz_u64_to_u32(val));
    }

    return (seq_t)(base + maxv2);
}
