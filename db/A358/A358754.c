/*
 * A358754  a(n) = 1 if A053669(n) [the smallest prime not dividing n] is of
 *          the form 6m+1, otherwise a(n) = 0.
 * https://oeis.org/A358754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9867, u16 n <= 9867, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = 1 iff the smallest prime that does NOT divide n
   is congruent to 1 modulo 6, otherwise 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static const uint64_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL
};
#define NUM_PRIMES (sizeof(primes) / sizeof(primes[0]))

seq_t a(idx_t n)
{
    for (idx_t i = 0; i < (idx_t)NUM_PRIMES; ++i) {
        if ((n % primes[i]) != 0ULL) {
            uint64_t p = primes[i];
            return (seq_t)((p % 6ULL) == 1ULL ? 1 : 0);
        }
    }
    /* In practice this line should never be reached because n cannot be
       divisible by all primes up to 53 on 64‑bit systems.  Return 0 as a
       safe fallback. */
    return 0;
}
