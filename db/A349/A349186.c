/*
 * A349186  G.f. A(x) satisfies: A(x) = (1 - x) / (1 - 2 * x - x^2 - x^3 *
 *          A(x)).
 * https://oeis.org/A349186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1; a(2)=3;
   a(n) = 2*a(n-1) + a(n-2) + Σ_{k=0}^{n-3} a(k)*a(n-3-k)   for n≥2 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms   */
static size_t cache_len = 0;      /* allocated length of cache */
static idx_t cache_used = 0;      /* number of terms that are valid */

/* Ensure the cache array can hold terms up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_len)
        return;

    if (cache == NULL) {
        cache = (seq_t *)malloc(required * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – terminate the program. */
            exit(1);
        }
        cache = tmp;
    }
    cache_len = required;
}

seq_t a(idx_t n)
{
    ensure_cache(n);

    for (idx_t i = cache_used; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 3;
        } else {
            seq_t sum = 0;
            /* Convolution term Σ_{k=0}^{i-3} a(k)*a(i-3-k) */
            if (i >= 3) {
                size_t limit = (size_t)i - 3;
                for (size_t k = 0; k <= limit; ++k) {
                    sum += cache[k] * cache[limit - k];
                }
            }
            seq_t tmp = cache[i - 1] << 1;   /* 2*a(i-1) */
            tmp += cache[i - 2];
            tmp += sum;
            cache[i] = tmp;
        }
    }

    cache_used = n + 1;
    return cache[n];
}
