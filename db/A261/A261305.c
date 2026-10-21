/*
 * A261305  a(n+1) = abs(a(n) - gcd(a(n), 5*n+4)), u(1) = 1.
 * https://oeis.org/A261305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |a(n-1) - gcd(a(n-1), 5*(n-1)+4)|, with a(1)=1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* n is 1‑based, offset 1 */
    if (n == 0) return 0;   /* not used by OEIS entry */

    /* grow cache if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        if (!cache) return 0;
        cache_size = 0;
    } else if (cache_size < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (!tmp) return 0;
        cache = tmp;
    }

    /* initialise first term a(1)=1 */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* i is zero‑based index of a(i+1).  i-1 refers to a(i). */
        seq_t prev = cache[i - 1];                    /* a(i)   */
        seq_t term = (seq_t)5 * (seq_t)i + (seq_t)4;  /* 5*i + 4   */
        seq_t g    = gcd(prev, term);
        seq_t diff = (prev >= g) ? prev - g : g - prev; /* |prev - g| */
        cache[i]   = diff;                            /* a(i+1)  */
    }
    cache_size = (size_t)n;

    return cache[n - 1];
}
