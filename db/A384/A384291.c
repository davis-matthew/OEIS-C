/*
 * A384291  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (214013*s+13737667) mod 2^24 when
 *          started at s=1.
 * https://oeis.org/A384291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n) = (214013*a(n-1)+13737667) mod 2^24 (n>1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Compute the nth term (1‑indexed). Returns 0 for n==0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined, but safe for callers */

    uint64_t val = 1;                  /* a(1) */
    if (n == 1) return (seq_t)val;

    const uint64_t mult = 214013ULL;
    const uint64_t incr = 13737667ULL;
    const uint64_t mask = (1ULL << 24) - 1ULL;   /* 2^24‑1 */

    for (idx_t i = 1; i < n; ++i)
        val = (mult * val + incr) & mask;

    return (seq_t)val;
}
