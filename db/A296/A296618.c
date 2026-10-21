/*
 * A296618  Expansion of the e.g.f. exp(-x)/sqrt(1-4*x).
 * https://oeis.org/A296618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296618
   Recurrence: a(0)=1, a(1)=1,
               a(n+2) = (4*n+5)*a(n+1) + 4*(n+1)*a(n)   for n >= 0
   The implementation iteratively computes terms and memoises them.
*/
#include <stddef.h>
#include <stdlib.h>

/* memoised cache for computed terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t  capacity = 0;         /* allocated size of cache */
static size_t  computed = 0;         /* number of terms stored (cache[0..computed-1]) */

/* ensure cache has space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= capacity)
        return;
    /* allocate with a quadratic growth to keep realloc inexpensive */
    size_t new_cap = needed * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* out of memory */
    cache = new_cache;
    capacity = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* return cached value if already computed */
    if (n < computed)
        return cache[n];

    /* make sure we have space for at least n+1 entries */
    ensure_capacity(n);

    /* initialise first two terms if not already done */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }
    if (n == 0)
        return cache[0];

    if (computed == 1) {
        cache[1] = (seq_t)1;
        computed = 2;
    }
    if (n == 1)
        return cache[1];

    /* compute terms iteratively up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* i >= 2: use recurrence with index m = i-2 */
        idx_t m = i - 2;
        seq_t term1 = ((seq_t)(4 * m + 5)) * cache[i - 1];
        seq_t term2 = ((seq_t)(4 * (m + 1))) * cache[i - 2];
        cache[i] = term1 + term2;
    }
    computed = (size_t)n + 1;
    return cache[n];
}
