/*
 * A248409  Least prime of the form x^2+13*n^2.
 * https://oeis.org/A248409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 69, u32 n <= 296, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least prime of the form x^2 + 13*n^2 (OEIS A248409) */
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* the OEIS offset is 1, so a(1) corresponds to n == 1 */
    if (n == 0) return 0;      /* value for n == 0 is irrelevant */

    /* work in 64‑bit arithmetic to avoid overflow for the expected range */
    uint64_t un   = (uint64_t)n;
    uint64_t base = 13ULL * un * un;   /* 13*n^2 */

    uint64_t x = 0;          /* start with x = 0 */
    for (;; ++x)
    {
        uint64_t val  = base + x * x;   /* candidate prime */
        seq_t cand = (seq_t)val;        /* cast to seq_t for the test */

        if (is_prime_big(cand))            /* true prime found */
            return cand;
        /* otherwise continue searching with the next x */
    }
}
