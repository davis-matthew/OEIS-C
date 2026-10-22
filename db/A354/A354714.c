/*
 * A354714  Nonprime numbers k such that the number of divisors of k divides
 *          k+1.
 * https://oeis.org/A354714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 8808, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements OEIS Sequence A354714
 *
 *  A nonprime integer k for which the number of divisors d(k)
 *  divides k+1.  The sequence is 1‑based:  a(1)=1, a(2)=15, ...
 *  The algorithm simply walks through the integers, tests the
 *  property, and caches the qualifying numbers.  The cache is
 *  grown using the provided seq_realloc helper so that the code
 *  works correctly with both plain integer types and the
 *  arbitrary‑precision Seq class.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

static seq_t *cache        = NULL;  /* array of discovered terms  */
static size_t cache_cap    = 0;     /* current allocated capacity */
static size_t cache_size   = 0;     /* number of stored terms      */
static idx_t  tested_until = 0;     /* last integer examined       */

/* Ensure cache can hold at least 'needed' elements.  */
static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    seq_t *tmp = seq_realloc(cache, cache_cap, needed);
    if (!tmp)            /* terminate on allocation failure   */
        abort();
    cache      = tmp;
    cache_cap  = needed;
}

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    size_t target = (size_t)n;      /* target index (1‑based) */

    /* Expand the cache until it contains the requested term. */
    while (cache_size < target) {
        ++tested_until;
        uint64_t k = (uint64_t)tested_until;

        /* Test the current integer k. */
        if (!is_prime(k)) {                 /* k is nonprime        */
            uint64_t d = divisor_count_u64(k);
            if (d != 0 && ((k + 1) % d) == 0) {   /* property holds */
                ensure_capacity(cache_size + 1);
                cache[cache_size] = (seq_t)k;     /* store the term    */
                ++cache_size;
            }
        }
    }

    return cache[target - 1];
}
