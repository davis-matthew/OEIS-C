/*
 * A365252  G.f. satisfies A(x) = 1 + x*A(x)*(1 + x^4*A(x)^2).
 * https://oeis.org/A365252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(0) = 1
 * For n >= 1:  a(n) = a(n-1) + (n >= 5 ? c[n-5] : 0)
 * where  c(m) = sum_{i=0}^m a[i] * b[m-i]
 * and  b(m) = sum_{j=0}^m a[j] * a[m-j].
 * All arrays are filled incrementally.
 */
#include <stdlib.h>

static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static seq_t *c_cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* initialise caches if first call */
    if (a_cache == NULL)
    {
        cache_size = 1;                 /* we already know a[0] */
        a_cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        b_cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        c_cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);

        /* base values for index 0 */
        a_cache[0] = 1;
        b_cache[0] = a_cache[0] * a_cache[0];   /* =1 */
        c_cache[0] = a_cache[0] * b_cache[0];   /* =1 */
    }

    /* grow the cache if needed */
    if ((idx_t)cache_size <= n)
    {
        size_t new_size = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        b_cache = (seq_t *)realloc(b_cache, new_size * sizeof(seq_t));
        c_cache = (seq_t *)realloc(c_cache, new_size * sizeof(seq_t));

        /* compute values for new indices */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i)
        {
            /* compute a[i] */
            seq_t prev = a_cache[i - 1];
            seq_t new_val = prev;
            if (i >= 5)
                new_val = new_val + c_cache[i - 5];
            a_cache[i] = new_val;

            /* compute b[i] = sum_{j=0}^i a[j] * a[i-j] */
            seq_t sum_b = 0;
            for (idx_t j = 0; j <= i; ++j)
                sum_b += a_cache[j] * a_cache[i - j];
            b_cache[i] = sum_b;

            /* compute c[i] = sum_{j=0}^i a[j] * b[i-j] */
            seq_t sum_c = 0;
            for (idx_t j = 0; j <= i; ++j)
                sum_c += a_cache[j] * b_cache[i - j];
            c_cache[i] = sum_c;
        }
        cache_size = new_size;
    }

    return a_cache[n];
}
