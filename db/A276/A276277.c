/*
 * A276277  Association types for monomials with n arguments in an algebra
 *          with two binary operations, one commutative, one noncommutative.
 * https://oeis.org/A276277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{n-1} a(i)a(n-i) // black root +
 *            Σ_{i=1}^{⌊(n-1)/2⌋} a(i)a(n-i) // white root, i< n-i + [n even]
 *            * a(n/2)(a(n/2)+1)/2 // white root, i = n-i
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for computed terms; indices 0..max_computed are valid */
static seq_t *cache          = NULL;
static idx_t  cache_capacity = 0;   /* allocated size (>= max_computed+1) */
static idx_t  max_computed   = 0;   /* highest index for which cache[i] is set */

/* Ensure that cache has space for indices 0..n inclusive. */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;                /* we need indices 0..n inclusive */
    if (needed <= cache_capacity) return;

    /* Allocate new array */
    seq_t *new_arr = (seq_t *)malloc((size_t)needed * sizeof(seq_t));
    if (!new_arr) abort();              /* in case of out‑of‑memory */

    /* Copy existing values if any */
    if (cache != NULL) {
        for (idx_t i = 0; i <= max_computed; ++i)
            new_arr[i] = cache[i];
        free(cache);
    } else {
        /* Nothing to copy; leave new_arr[0 .. max_computed] uninitialized
           but they will be set before use. */
    }

    /* Initialize newly allocated slots to zero for safety */
    for (idx_t i = max_computed + 1; i < needed; ++i)
        new_arr[i] = 0;

    cache = new_arr;
    cache_capacity = needed;
}

/* Sequence a(n) = number of association types for n arguments. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* values below the offset are undefined */

    /* If already computed, return cached value */
    if (n <= max_computed) return cache[n];

    /* Make sure the cache is large enough */
    ensure_cache(n);

    /* Initialise base cases */
    if (max_computed == 0) {
        cache[0] = 0;           /* unused, but set for safety */
        cache[1] = 1;           /* a(1) = 1 */
        max_computed = 1;
    }

    /* Compute terms from max_computed+1 up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t sum_black = 0;
        seq_t sum_white = 0;

        /* Black root: ordered pairs (i, m-i) */
        for (idx_t i = 1; i <= m - 1; ++i)
            sum_black += cache[i] * cache[m - i];

        /* White root: unordered pairs with i < m-i */
        for (idx_t i = 1; i <= (m - 1) / 2; ++i)
            sum_white += cache[i] * cache[m - i];

        /* White root with equal subtrees (only when m even) */
        if ((m & 1ULL) == 0) {
            idx_t k = m / 2;
            sum_white += cache[k] * (cache[k] + 1) / 2;
        }

        cache[m] = sum_black + sum_white;
    }

    max_computed = n;
    return cache[n];
}
