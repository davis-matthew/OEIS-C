/*
 * A307464  Number of Catalan words of length n avoiding the pattern 000.
 * https://oeis.org/A307464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) (n ≥ 3) initial values: a(0) =
 *            1, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute a(n), number of Catalan words of length n avoiding the pattern 000 */
#include <stdlib.h>
#include <stdint.h>

/* cache of previously computed terms */
static seq_t *cache = NULL;    /* array of a(i) values */
static idx_t cache_cap = 0;    /* allocated size (number of elements) */
static idx_t last_computed = 0;/* highest index that contains a valid value */

/* ensure that cache can hold indices up to 'need' (inclusive) */
static void
ensure_capacity(idx_t need)
{
    idx_t req = need + (idx_t)1;
    if (req <= cache_cap)
        return;

    /* first allocation or resizing */
    if (cache == NULL) {
        if (req < (idx_t)3)
            req = (idx_t)3;              /* at least space for the base cases */

        cache = (seq_t *)malloc(req * sizeof(seq_t));
        if (!cache)
            abort();                     /* allocation failure */

        /* initialise base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_cap = req;
        last_computed = (idx_t)2;
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, req * sizeof(seq_t));
        if (!tmp)
            abort();                     /* allocation failure */

        cache = tmp;
        cache_cap = req;
        /* existing values are preserved; last_computed stays unchanged */
    }
}

/* OEIS A307464: number of Catalan words of length n avoiding the pattern 000 */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    if (n <= last_computed)
        return cache[(size_t)n];

    for (idx_t i = last_computed + (idx_t)1; i <= n; ++i) {
        if (i >= (idx_t)3) {
            cache[(size_t)i] = cache[(size_t)(i - (idx_t)1)]
                            + (seq_t)3 * cache[(size_t)(i - (idx_t)2)]
                            - cache[(size_t)(i - (idx_t)3)];
        }
        /* i < 3 should not occur because base values are already set */
    }

    last_computed = n;
    return cache[(size_t)n];
}
