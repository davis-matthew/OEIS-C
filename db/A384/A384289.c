/*
 * A384289  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for GWBASIC 3.23 when started at 1.
 * https://oeis.org/A384289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n) = (214013 * a(n-1) + 2531011) mod 2^24 for n≥2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2^24 modulus; use a mask for speed */
    const uint64_t MODMASK = 0xFFFFFFULL; /* 2^24 - 1 */

    /* The sequence is periodic with period 2^24. Reduce n modulo that period */
    if (n >= 16777216ULL) /* period = 2^24 */
        n = ((n - 1ULL) % (16777216ULL)) + 1ULL;   /* keep 1‑based index */

    if (n == 0ULL)        /* offset is 1; legal callers use n≥1 */
        return 0L;         /* undefined, but protect against wrap‑around */

    seq_t value = 1;      /* a(1) */

    if (n == 1ULL)
        return value;

    const uint64_t K = 214013ULL;
    const uint64_t C = 2531011ULL;

    /* Iterate plainly; n ≤ 2^24, so at most 16 777 15 steps */
    for (idx_t i = 2; i <= n; ++i)
    {
        uint64_t temp = (K * (uint64_t)value + C) & MODMASK;
        value = (seq_t)temp;
    }

    return value;
}
