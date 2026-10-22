/*
 * A330958  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the least possible value of x OR y (where OR denotes the bitwise
 *          OR operator).
 * https://oeis.org/A330958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for A330958:
   a(n) = minimum value of (x | y) over all pairs (x, y) such that the
   binary representation of n can be obtained by interleaving the bits
   of x and y (preserving the order within each number). */
/* The algorithm enumerates all 2^k assignments of the k bits of n to
   either x or y, where k is the length of the binary representation of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t L;
    if (n == 0) {
        L = 1;               /* handle n = 0  -> binary "0"  */
    } else {
        /* number of bits in n: 64 - clz(n) */
        L = 64 - __builtin_clzll((unsigned long long)n);
    }

    /* use 64‑bit accumulators to avoid overflow on small seq_t types */
    unsigned long long best = ~0ULL;   /* initialise to max 64‑bit value */
    unsigned long long mask_limit = 1ULL << L;

    for (unsigned long long mask = 0; mask < mask_limit; ++mask) {
        unsigned long long x = 0, y = 0;

        for (idx_t i = 0; i < L; ++i) {
            /* bit of n at position i (most significant first) */
            idx_t bit = (n >> (L - 1 - i)) & 1U;

            if (mask & (1ULL << (L - 1 - i))) {
                x = (x << 1) | (unsigned long long)bit;
            } else {
                y = (y << 1) | (unsigned long long)bit;
            }
        }

        unsigned long long val = x | y;
        if (val < best) {
            best = val;
        }
    }

    return (seq_t)best;
}
