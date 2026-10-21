/*
 * A276416  a(n) = a(n-1)*(1 + a(n-1)/a(n-4)), with a(0) = a(1) = a(2) = a(3)
 *          = 1.
 * https://oeis.org/A276416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * (1 + a(n-1)/a(n-4)), with a(0) = a(1) = a(2) =
 *            a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t max_calculated = (idx_t)-1; /* indicates nothing computed yet */

static void ensure_value(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cache_size) {
        size_t new_cap = cache_size == 0 ? 4 : cache_size;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1; /* double until enough */
        }
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!new_cache) {
            /* allocation failed; keep old cache unchanged */
            return;
        }
        cache = new_cache;
        cache_size = new_cap;
    }

    /* Compute missing entries */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)1;
        } else {
            seq_t prev  = cache[i - 1];
            seq_t prev4 = cache[i - 4];
            seq_t div   = prev / prev4;           /* exact integer division */
            cache[i] = prev * ((seq_t)1 + div);
        }
    }
    if (n > max_calculated) {
        max_calculated = n;
    }
}

seq_t a(idx_t n)
{
    ensure_value(n);
    return cache[n];
}
