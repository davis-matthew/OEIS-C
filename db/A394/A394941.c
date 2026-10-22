/*
 * A394941  Number of well-formed bracketed words of total length n built from
 *          the symbol * (of length 1) and four unary bracket types, with no
 *          empty bracket pair.
 * https://oeis.org/A394941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=1 a(n)=a(n-1)+4*a(n-2)+4*∑_{i=1}^{n-3}
 *            a(i)*a(n-2-i) for n≥3 The sequence is memoised in a static
 *            cache. All arithmetic uses the seq_t type exclusively, without
 *            any floating point or built‑in integer casts that would break on
 *            the arbitrary‑precision build. The implementation runs i...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  len   = 0;

    /* Grow the cache to hold indices 0..n */
    if (n + 1 > len) {
        idx_t newlen = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)newlen);
        if (!tmp) abort();                 /* memory exhausted – fail fast */
        cache = tmp;
        /* initialise new slots with zero */
        for (idx_t i = len; i < newlen; ++i) cache[i] = 0;
        len = newlen;
    }

    /* Compute all terms up to n if not already present */
    for (idx_t i = 0; i <= n; ++i) {
        /* Base values */
        if (i <= 2) {
            if (cache[i] == 0) cache[i] = (seq_t)1;
            continue;
        }

        /* Skip if already computed */
        if (cache[i] != 0) continue;

        seq_t term1 = cache[i-1];
        seq_t term2 = (seq_t)4 * cache[i-2];

        seq_t sum = 0;
        for (idx_t j = 1; j <= i-3; ++j) {
            sum += cache[j] * cache[i-2-j];
        }

        seq_t sum4 = (seq_t)4 * sum;
        cache[i] = term1 + term2 + sum4;
    }

    return cache[n];
}
