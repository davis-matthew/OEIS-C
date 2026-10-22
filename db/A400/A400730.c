/*
 * A400730  Numbers k such that k^2 + 1 is an exponentially odd number
 *          (A268335).
 * https://oeis.org/A400730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computed by brute‑force: check successive k for which k^2+1 has only odd
 * prime exponents (i.e., is exponentially odd).  a(1) = 1, a(2) = 2, ...
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* Indexing starts at 1 (offset 1). */
    idx_t found = 0;

    for (idx_t k = 1; ; ++k) {
        uint64_t sq = (uint64_t)k * (uint64_t)k + 1ULL;

        /* Factor sq.  The maximum number of distinct prime factors for sq
           is very small; 64 slots are far more than sufficient. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t count = 0;
        factor_u64(sq, primes, exps, &count);

        bool ok = true;
        for (size_t i = 0; i < count; ++i) {
            if ((exps[i] & 1ULL) == 0ULL) {    /* even exponent found */
                ok = false;
                break;
            }
        }

        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
