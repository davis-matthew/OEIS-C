/*
 * A309023  For n >= 1, if there exists an m < n such that a(m) = a(n), take
 *          the largest such m and set a(n+1) = (n-m)^a(n); otherwise a(n+1) =
 *          0. Start with a(1)=1, a(2)=2.
 * https://oeis.org/A309023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1, a(2) = 2. * For n ≥ 1 let m be the largest index < n
 *            with a(m) = a(n). * If such m exists, a(n+1) = (n-m) ^ a(n);
 *            otherwise a(n+1) = 0
 * Range:     u8 n <= 16, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_realloc.h"

static seq_t *cache      = NULL;  /* cache[0] = a(1), cache[1] = a(2), … */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t computed   = 0;      /* number of terms that have been computed */

/* Ensure the cache can store at least n terms (1‑based indexing). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n==0, return 0 */

    /* Initialise the first two terms on the first call. */
    if (computed == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;       /* a(1) */
        cache[1] = (seq_t)2;       /* a(2) */
        computed = 2;
    }

    /* Return the cached value if already computed. */
    if ((size_t)n <= computed)
        return cache[n - 1];

    /* Compute terms up to n. */
    ensure_capacity(n);
    for (size_t cur = computed + 1; cur <= (size_t)n; ++cur) {
        /* cur is the index of the term being computed: a(cur). */
        seq_t prev = cache[cur - 2];           /* a(cur-1) */
        size_t m = 0;
        for (size_t j = cur - 2; j > 0; --j) {
            if (cache[j - 1] == prev) {
                m = j;
                break;
            }
        }
        if (m == 0) {
            cache[cur - 1] = (seq_t)0;
        } else {
            seq_t base = (seq_t)(cur - 1 - m); /* (n-m) where n = cur-1 */
            cache[cur - 1] = big_pow(base, prev);
        }
    }
    computed = n;
    return cache[n - 1];
}
