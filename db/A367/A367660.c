/*
 * A367660  G.f. A(x) satisfies A(x) = 1 / (1 - x - x * A(x^4)).
 * https://oeis.org/A367660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = a(n-1) + Σ_{k=0}^{⌊(n-1)/4⌋} a(k) *
 *            a(n-1-4k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cap = 0;               /* current capacity of cache */
static idx_t last_computed = 0;      /* highest index that has been computed */

/* ensure the cache can hold values up to n */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cap > n)
        return;

    size_t new_cap = cap ? cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_buf = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_buf)
        exit(EXIT_FAILURE);          /* allocation failed */

    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            new_buf[i] = cache[i];
        free(cache);
    }
    cache = new_buf;
    cap = new_cap;

    /* base value */
    if (cap > 0 && cache[0] == 0)
        cache[0] = 1;
}

/* main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    ensure_cache(n);

    if (cache[n] != 0)
        return cache[n];              /* already computed */

    /* compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1];     /* a(i-1) */

        idx_t max_k = (i - 1) / 4;    /* floor((i-1)/4) */
        for (idx_t k = 0; k <= max_k; ++k) {
            idx_t j = i - 1 - 4 * k;  /* n-1-4k */
            sum += cache[k] * cache[j];
        }
        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
