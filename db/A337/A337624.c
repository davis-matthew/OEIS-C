/*
 * A337624  a(n) is the least positive integer in base n that when multiplied
 *          by 2n-1 will contain only the digits 0 and 1.
 * https://oeis.org/A337624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A337623(n)/(2*n-1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   compute_single
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A337623(n) is the least positive integer in base n
 *   using only the digits 0 and 1 that is divisible by (2*n-1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_single.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    size_t new_size = (size_t)n + 1;          /* indices 0..n; we use 1..n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                  /* allocation failure */
    /* initialise newly added entries (indices cache_size+1 .. n) */
    for (idx_t i = cache_size + 1; i < new_size; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n=0, safe guard */
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];  /* memoised result */
    cache[n] = compute_single(n);
    return cache[n];
}
