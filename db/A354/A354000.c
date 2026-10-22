/*
 * A354000  Expansion of e.g.f. exp(x^2/2 * (exp(x) - 1)).
 * https://oeis.org/A354000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=2..n-1} binom(n-1, j) * ((j+1)*j/2) * a(n-j-1)
 *            with a(0)=1, a(1)=0, a(2)=0. This follows from the given
 *            recurrence
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The following static variables will hold the memoized values. */
static seq_t *cache = NULL;          /* cached a(n) values */
static idx_t cache_capacity = 0;     /* allocated size in terms of indices */
static idx_t last_computed = 0;      /* highest n computed so far */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_capacity <= n) {
        idx_t new_cap = cache_capacity ? cache_capacity : 1;
        while (new_cap <= n) new_cap <<= 1;
        /* Reallocate the array.  We cast the void* to seq_t*;
           the build supplies a trivial constructor for seq_t, or
           an arbitrary‑precision class that supports placement assignment. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }
}

/* Initialise the cache with the base values a(0)=1, a(1)=0, a(2)=0. */
static void init_cache(void)
{
    if (cache == NULL) {
        ensure_cache_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        last_computed = 2;
    }
}

/* ------------------------------------------------------------ */
/* Compute a(n) with memoisation and the recurrence proved above. */
seq_t a(idx_t n)
{
    init_cache();                        /* make sure base cases exist */

    if (n <= last_computed)              /* already computed */
        return cache[n];

    ensure_cache_capacity(n);            /* grow array if necessary */

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* computation of a(i) */
        seq_t sum = (seq_t)0;
        /* binom_prev holds binom(i-1, j-1); start with j=2 => binom(i-1,1)=i-1 */
        seq_t binom_prev = (seq_t)(i - 1);

        for (idx_t j = 2; j <= i - 1; ++j) {
            idx_t rem = i - j - 1;          /* n-k where k=j+1 */

            /* binom(i-1,j) = binom_prev * (i-j) / j */
            seq_t binom = binom_prev * (seq_t)(i - j);
            binom = binom / (seq_t)j;

            /* ((j+1)*j)/2 */
            seq_t mult = (seq_t)( (j + 1) * j / 2 );

            seq_t term = binom * mult;
            term = term * cache[rem];

            sum += term;

            binom_prev = binom;
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
