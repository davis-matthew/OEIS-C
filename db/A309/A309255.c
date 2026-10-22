/*
 * A309255  a(n) = n + 1 - Sum_{k=0..n} (Stirling1(n,k) mod 2).
 * https://oeis.org/A309255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 - 2^popcount(floor(n/2))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute number of 1-bits in a 64‑bit unsigned integer. */
static inline unsigned int popcount_u64(uint64_t x) {
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    idx_t half = n >> 1;                 /* floor(n/2)            */
    unsigned int bits = popcount_u64((uint64_t)half);
    seq_t pow2 = ((seq_t)1 << bits);     /* 2^bits                */
    seq_t result = (seq_t)n + 1 - pow2;  /* n + 1 - 2^bits        */
    return result;
}
