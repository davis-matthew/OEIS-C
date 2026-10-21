/*
 * A255817  Parity of A000788, which is the total number of ones in 0..n in
 *          binary.
 * https://oeis.org/A255817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} popcount(k) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: floor(log2(v)) for 64‑bit unsigned integers.
   Works for v>0.  In C++ this is still valid. */
static inline unsigned floor_log2_uint64(uint64_t v)
{
    /* __builtin_clzll is defined for unsigned long long (>=64 bits). */
    return 63U - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    /* The result is always 0 or 1, so we can compute it in a
       native unsigned type and then cast to seq_t. */
    unsigned res = 0;               /* parity accumulator */
    uint64_t x = (uint64_t)n;       /* work on 64‑bit value */

    while (x != 0)
    {
        unsigned k = floor_log2_uint64(x);   /* highest set bit */
        if (k == 1)                          /* f(2^k-1) == 1 only for k==1 */
            res ^= 1;

        uint64_t m = 1ULL << k;              /* 2^k */
        uint64_t y = x - m;                  /* n - 2^k */

        res ^= (unsigned)((y + 1) & 1ULL);    /* parity of (y+1) */

        x = y;                               /* recurse on the lower part */
    }

    return (seq_t)res;
}
