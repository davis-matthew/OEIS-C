/*
 * A306347  Expansion of e.g.f. exp((sin(x) + sinh(x))/2).
 * https://oeis.org/A306347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..⌊(n-1)/4⌋} binomial(n-1, 4k) *
 *            a(n-4k-1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 20, u64 n <= 31, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* multiplicative binomial coefficient, with idx_t cast to seq_t where needed */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

/* dynamic cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cap   = 0;     /* allocated size of cache array   */
static size_t last  = 0;     /* highest index computed (0‑based) */

static void ensure_cache(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();            /* abort on allocation failure */
    cache = new_ptr;
    /* zero the newly allocated part if cache was grown */
    for (size_t i = cap; i < new_cap; ++i) cache[i] = 0;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* initialise cache on first use */
    if (!cache) {
        ensure_cache(1);
        cache[0] = 1;
        last = 0;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t bin = binom(i - 1, 4 * k);
                sum += bin * cache[i - 4 * k - 1];
            }
            ensure_cache(i + 1);
            cache[i] = sum;
        }
        last = n;
    }
    return cache[n];
}
