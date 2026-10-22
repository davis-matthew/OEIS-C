/*
 * A337196  The 3-adic valuation of 1+A000265(sigma(n)), where A000265 gives
 *          the odd part.
 * https://oeis.org/A337196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v3(1 + A000265(σ(n))) (v3 = 3‑adic valuation)
 * Range:     u8 n <= 687, u16 n <= 9852, u32 n <= 9852, u64 n <= 9852,
 *            u128 n <= 9852, big any n
 * Helpers:   alloc_seq_array, free_seq_array, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* for NULL */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/odd_part.h"

static seq_t *sigma_cache = NULL;   /* cache of σ(1..current_max)      */
static idx_t  sigma_max  = 0;       /* size of current cache (excl. 0) */

/* return odd part of x by stripping factors of 2 */

/* ensure sigma_cache[1..n] contains σ(1)..σ(n) */
static void
ensure_sigma(idx_t n)
{
    if (n <= sigma_max)
        return;

    /* allocate new cache of size n+1 (index 0 unused) */
    seq_t *new_cache = alloc_seq_array(n + 1);
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = 0;

    /* classic divisor‑sum sieve */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t si = (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            new_cache[j] += si;
    }

    if (sigma_cache)
        free_seq_array(sigma_cache);
    sigma_cache = new_cache;
    sigma_max   = n;
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined for n=0, return 0 safely */
        return 0;

    ensure_sigma(n);

    seq_t sigma_val = sigma_cache[n];
    seq_t val       = odd_part(sigma_val) + 1;

    seq_t v3 = 0;
    while (val % 3 == 0) {
        val /= 3;
        ++v3;
    }
    return v3;
}
