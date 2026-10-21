/*
 * A276261  Centered 21-gonal primes.
 * https://oeis.org/A276261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): primes of the form (21*k^2 + 21*k + 2)/2.  a(n) is the
   n-th such prime, for n ≥ 1.  The function builds the sequence as
   needed and caches its values for subsequent calls. */
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Cache of found primes.  Static state persists across calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    /* Last k that was examined.  Initialized to (idx_t)-1 so that
       the first increment yields k = 0. */
    static idx_t last_k = (idx_t)-1;

    if (n == 0)     /* offset not used – behaviour irrelevant for n=0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while ((idx_t)cache_size < n)
    {
        idx_t k = ++last_k;
        seq_t kt = (seq_t)k;
        /* Compute (21*k^2 + 21*k + 2)/2 in seq_t. */
        seq_t numerator = kt * kt * 21 + kt * 21 + 2;
        seq_t value      = numerator / 2;
        if (isPrimeGeneric(value))
            append_candidate(&cache, &cache_size, &cache_cap, value);
    }
    return cache[n - 1];
}
