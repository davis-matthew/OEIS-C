/*
 * A276960  a(n) = A000262(n)^2.
 * https://oeis.org/A276960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000262(n)^2 The sequence satisfies the 3‑term recurrence
 *            (2*n+3)*a(n+3)-(2*n+5)*(3*n^2+13*n+13)*a(n+2)
 *            +(n+2)*(n+1)*(2*n+3)*(3*n^2+13*n+13)*a(n+1)
 *            -n^2*(n+1)^3*(n+2)*(2*n+5)*a(n) = 0 with initial terms a(0)=1,
 *            a(1)=1, a(2)=9, a(3)=169
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_cap = 0;         /* current array capacity           */
static idx_t cache_last = 0;         /* largest index already computed   */
static int cache_initialized = 0;    /* flag to initialise base values   */

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (!cache_initialized)
    {
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = (seq_t)1;
        cache[1]    = (seq_t)1;
        cache[2]    = (seq_t)9;
        cache[3]    = (seq_t)169;
        cache_last  = 3;
        cache_initialized = 1;
    }

    /* Return already computed value */
    if (n <= cache_last)
        return cache[n];

    /* Enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence, starting at 4 */
    idx_t start = cache_last + 1;
    if (start < 4) start = 4;
    for (idx_t k = start; k <= n; ++k)
    {
        idx_t i = k - 3;              /* index used in recurrence */

        /* Common sub‑expression: c = 3*i^2 + 13*i + 13 */
        seq_t c = (seq_t)(3 * i * i + 13 * i + 13);

        /* term1 = (2i+5)*c*a[i+2] */
        seq_t term1 = cache[i + 2];
        term1 = term1 * ((seq_t)(2 * i + 5));
        term1 = term1 * c;

        /* term2 = (i+2)*(i+1)*(2i+3)*c*a[i+1]  (subtracted) */
        seq_t term2 = cache[i + 1];
        term2 = term2 * ((seq_t)(i + 2));
        term2 = term2 * ((seq_t)(i + 1));
        term2 = term2 * ((seq_t)(2 * i + 3));
        term2 = term2 * c;

        /* term0 = i^2*(i+1)^3*(i+2)*(2i+5)*a[i]  (added) */
        seq_t term0 = cache[i];
        term0 = term0 * ((seq_t)i);
        term0 = term0 * ((seq_t)i);
        term0 = term0 * ((seq_t)(i + 1));
        term0 = term0 * ((seq_t)(i + 1));
        term0 = term0 * ((seq_t)(i + 1));
        term0 = term0 * ((seq_t)(i + 2));
        term0 = term0 * ((seq_t)(2 * i + 5));

        /* numerator = term1 - term2 + term0 */
        seq_t numerator = term1;
        numerator -= term2;
        numerator += term0;

        /* Divide by (2i+3) to obtain a[k] */
        seq_t denom = (seq_t)(2 * i + 3);
        cache[k] = numerator / denom;
    }

    cache_last = n;
    return cache[n];
}
