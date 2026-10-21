/*
 * A256272  G.f.: Sum_{n>=1} Pell(n+1) * x^n / (1 - x^n), where Pell(n) =
 *          A000129(n).
 * https://oeis.org/A256272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} Pell(d+1) (Pell(0)=0, Pell(1)=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=2, a(2)=7, a(3)=14, … (values from OEIS A256272) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache_a = NULL;   /* a[1..max_n]  (index 0 unused) */
static seq_t *cache_p = NULL;   /* Pell[0..max_n+1] */
static size_t cap_a  = 0;       /* current capacity of cache_a   */
static size_t cap_p  = 0;       /* current capacity of cache_p   */
static idx_t  max_n  = 0;       /* greatest n whose a(n) is cached */

/* Allocate or reallocate the cache arrays to hold up to n elements.  */
static void
resize_cache(idx_t n)
{
    size_t new_cap_a = (size_t)n + 1;     /* include index 0 */
    size_t new_cap_p = (size_t)n + 2;     /* need Pell[n+1] too */

#ifdef __cplusplus
    /* C++: use new[] / delete[] (objects may have constructors) */
    if (new_cap_a > cap_a) {
        seq_t *tmp = new seq_t[new_cap_a];
        delete [] cache_a;
        cache_a = tmp;
        cap_a  = new_cap_a;
    }
    if (new_cap_p > cap_p) {
        seq_t *tmp = new seq_t[new_cap_p];
        delete [] cache_p;
        cache_p = tmp;
        cap_p  = new_cap_p;
    }
#else
    /* C: use realloc (plain memory, seq_t must be trivially constructible) */
    if (new_cap_a > cap_a) {
        cache_a = (seq_t *)realloc(cache_a, new_cap_a * sizeof(seq_t));
        cap_a   = new_cap_a;
    }
    if (new_cap_p > cap_p) {
        cache_p = (seq_t *)realloc(cache_p, new_cap_p * sizeof(seq_t));
        cap_p   = new_cap_p;
    }
#endif
}

/* Precompute a[1..n] and Pell[0..n+1] if necessary. */
static void
compute_up_to(idx_t n)
{
    if (n <= max_n) return;   /* already computed and cached */

    resize_cache(n);

    /* Compute Pell numbers up to Pell[n+1]. */
    cache_p[0] = (seq_t)0;
    if (n >= 1) cache_p[1] = (seq_t)1;
    for (idx_t k = 2; k <= n + 1; ++k)
        cache_p[k] = (seq_t)(2 * cache_p[k - 1] + cache_p[k - 2]);

    /* Zero the a array (index 0 unused). */
    for (idx_t i = 1; i <= n; ++i)
        cache_a[i] = (seq_t)0;

    /* Accumulate the divisor sums. */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t val = cache_p[d + 1];                /* Pell(d+1) */
        for (idx_t m = d; m <= n; m += d)
            cache_a[m] += val;
    }

    max_n = n;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 (offset 1) */
    compute_up_to(n);
    return cache_a[n];
}
