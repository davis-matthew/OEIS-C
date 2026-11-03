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
 * Validated: standard, pass, 77 terms through n = 9917 (2026-11-01)
 * Helpers:   is_prime, num_divisors
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
#include "../utils/is_prime.h"
#include "../utils/num_divisors.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
            uint64_t d = num_divisors(k);
            if (d != 0 && ((k + 1) % d) == 0) {   /* property holds */
                ensure_capacity(cache_size + 1);
                cache[cache_size] = (seq_t)k;     /* store the term    */
                ++cache_size;
            }
        }
    }

    return cache[target - 1];
}
