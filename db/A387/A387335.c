/*
 * A387335  1, 2, 4, along with numbers divisible by at least two distinct
 *          primes.
 * https://oeis.org/A387335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is either 1, 2, 4, or has
 *            at least two distinct prime divisors. (OEIS A387335)
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"   /* count_distinct_primes() */
#include "../utils/grow_seq.h"                /* grow_seq() */

seq_t a(idx_t n)
{
    /* Static cache: index 0 corresponds to a(1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated capacity */
    static size_t len   = 0;  /* number of terms computed */
    static idx_t  next  = 1;  /* next integer to test */

    if (n == 0)               /* offset starts at 1; undef for 0 */
        return (seq_t)0;

    while (len < n) {
        seq_t val = (seq_t)next;

        /* Accept 1, 2, 4 or any number with at least two distinct prime factors */
        if (next == 1 || next == 2 || next == 4 ||
            count_distinct_primes((uint64_t)next) >= 2) {
            /* Ensure space for the new term */
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = val;
        }
        ++next;
    }

    return cache[n - 1];
}
