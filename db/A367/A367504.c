/*
 * A367504  a(1) = 2; for n > 1, a(n) = a(n-1) + 2*gpf(a(n-1)), where gpf(k) =
 *          A006530(k) = greatest prime dividing k.
 * https://oeis.org/A367504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n > 1, a(n) = a(n-1) + 2*gpf(a(n-1)), where gpf(k)
 *            is the greatest prime factor of k
 * Range:     u8 n <= 20, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/mem.h"

/* Static cache of computed terms */
static seq_t *cache = NULL;      /* pointer to cached terms */
static idx_t   cache_len = 0;    /* number of terms currently cached */
static size_t  cache_cap = 0;    /* capacity of the cache array */

/* Return the n‑th term of sequence A367504. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Grow the cache to hold at least n terms. */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)n);
        if (!tmp) return 0;          /* allocation failed – fall back to 0 */
        cache = tmp;
        cache_cap = (size_t)n;
    }

    /* Initialise the first term if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Fill missing terms up to index n. */
    while (cache_len < n) {
        seq_t prev  = cache[cache_len - 1];
        uint64_t gpf_val = largest_prime_factor((uint64_t)prev);
        seq_t    inc = ((seq_t)2) * (seq_t)gpf_val;
        cache[cache_len] = prev + inc;
        ++cache_len;
    }

    return cache[n - 1];
}
