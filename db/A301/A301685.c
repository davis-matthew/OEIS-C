/*
 * A301685  Partial sums of A301684.
 * https://oeis.org/A301685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) for n > 8,
 *            * a(0)=1, a(1)=5, a(2)=13, a(3)=25, a(4)=43, a(5)=65, * a(6)=93,
 *            a(7)=127, a(8)=163
 * Range:     u8 n <= 10, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache enables arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* First nine terms */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)5, (seq_t)13, (seq_t)25, (seq_t)43,
        (seq_t)65, (seq_t)93, (seq_t)127, (seq_t)163
    };

    /* Initialise cache if needed */
    if (!cache) {
        cache_len = 9;
        cache = seq_alloc((size_t)cache_len);
        if (!cache) abort();          /* allocation failure */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* Determine the lowest index that needs to be computed */
    idx_t start = cache_len;
    if (start < 9) start = 9;          /* after init cache_len may be 9 */

    /* Grow cache only if necessary, and remember the old size */
    idx_t old_len = cache_len;
    if (n >= start) {
        /* Ensure we have room for indices up to n */
        if (cache_len <= n) {
            idx_t new_len = n + 1;
            seq_t *tmp = seq_realloc(cache, (size_t)cache_len, (size_t)new_len);
            if (!tmp) abort();        /* allocation failed */
            cache = tmp;
            cache_len = new_len;
        }

        /* Compute missing values from old_len up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t val = cache[i-1];
            val += cache[i-1];            /* 2*a(i-1) */
            val -= cache[i-2];            /* -a(i-2) */
            val += cache[i-3];            /* +a(i-3) */
            val -= cache[i-4];            /* -a(i-4) */
            val -= cache[i-4];            /* -a(i-4) */
            val += cache[i-5];            /* +a(i-5) */
            cache[i] = val;
        }
    }

    return cache[n];
}
