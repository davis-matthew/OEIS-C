/*
 * A280439  Number of n X 7 0..1 arrays with no element equal to more than one
 *          of its horizontal and vertical neighbors, with the exception of
 *          exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) for
 *            n>7 with initial values a(1)=20, a(2)=120, a(3)=236, a(4)=386,
 *            a(5)=574, a(6)=834, a(7)=1211. All computations are exact in
 *            seq_t
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 37, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static const seq_t base_values[8] = {0, 20, 120, 236, 386, 574, 834, 1211};

static seq_t *cache = NULL;
static idx_t cache_max = 0;

seq_t a(idx_t n)
{
    /* Handle the offset.  a(1) is the first value.  */
    if (n == 0) return 0;

    /* For n <= 7 we can return the literal constant without initializing the
       cache.  */
    if (n <= 7) return base_values[(size_t)n];

    /* Ensure the cache is allocated and contains the first 7 terms.  */
    if (cache_max < 7) {
        size_t cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* abort on allocation failure */
        for (idx_t i = 1; i <= 7; ++i) cache[i] = base_values[(size_t)i];
        cache_max = 7;
    }

    /* Grow the cache if necessary and compute missing terms using the
       recurrence.  */
    if (n > cache_max) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > (size_t)(cache_max + 1)) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* pos = 4*a[k-1] + 4*a[k-4] */
            seq_t pos = (cache[k - 1] << 2) + (cache[k - 4] << 2);
            /* neg = 4*a[k-2] + 2*a[k-3] + a[k-6] */
            seq_t neg = (cache[k - 2] << 2) + (cache[k - 3] << 1) + cache[k - 6];
            cache[k] = pos - neg;    /* guaranteed non‑negative for the
                                        sequence definition */
        }
        cache_max = n;
    }

    return cache[n];
}
