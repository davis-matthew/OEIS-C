/*
 * A321503  Numbers m such that m and m+1 both have at least 3 distinct prime
 *          factors.
 * https://oeis.org/A321503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: numbers m such that both m and m+1 have at least 3 distinct prime factors. */
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

/* Return the n-th term (n>=1) of OEIS A321503.
   This function scans integers sequentially, counting those m that satisfy
   the condition.  It's deterministic, requires no memory allocation and
   works for any build of seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* behaviour below the offset is irrelevant */
    uint64_t m = 1;
    uint64_t placed = 0ULL;
    for (;; ++m) {
        if (count_distinct_primes(m)   >= 3 &&
            count_distinct_primes(m + 1) >= 3) {
            ++placed;
            if (placed == n)
                return (seq_t)m;
        }
    }
}
