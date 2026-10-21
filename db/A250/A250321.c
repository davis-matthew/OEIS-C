/*
 * A250321  Number of length 2+2 0..n arrays with the sum of second
 *          differences squared multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A250321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8) Initial values: a(1)=8, a(2)=25,
 *            a(3)=60, a(4)=117, a(5)=200, a(6)=321, a(7)=480, a(8)=681 This
 *            recurrence holds for all n ≥ 9. The implementation caches
 *            previously computed terms so that any call order is hand...
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *a_cache   = NULL;   /* cached values, index 0 corresponds to a(1) */
static idx_t a_cache_sz = 0;      /* current cache capacity (number of entries) */
static idx_t a_max_computed = 0;  /* largest n for which a(n) is already stored */

/* Ensure the cache can hold at least n values (n is 1‑based).  */
static void ensure_cache(idx_t n)
{
    if (n <= a_cache_sz) return;
    idx_t new_sz = n + 16;                /* extra room to avoid frequent reallocs */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_sz);
    if (!new_cache) return;               /* out of memory – caller will see 0 */
    if (a_cache) {
        memcpy(new_cache, a_cache, sizeof(seq_t) * a_cache_sz);
        free(a_cache);
    }
    a_cache = new_cache;
    a_cache_sz = new_sz;
    /* Initialise missing initial terms if needed.  */
    if (a_max_computed < 8) {
        seq_t init[8] = {8,25,60,117,200,321,480,681};
        idx_t i;
        for (i = 0; i < 8 && i < a_cache_sz; ++i) a_cache[i] = init[i];
        a_max_computed = 8;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* behaviour below the offset is unspecified */
    ensure_cache(n);

    if (n <= a_max_computed) return a_cache[n-1];

    /* Compute terms from a_max_computed+1 up to n */
    for (idx_t i = a_max_computed + 1; i <= n; ++i) {
        idx_t idx = i - 1;               /* 0‑based index in cache   */
        seq_t pos = a_cache[idx-1] * 2
                  + a_cache[idx-3] * 2
                  + a_cache[idx-5] * 2
                  + a_cache[idx-7] * 2;
        seq_t neg = a_cache[idx-2]
                  + a_cache[idx-4] * 4
                  + a_cache[idx-6]
                  + a_cache[idx-8];
        a_cache[idx] = pos - neg;
    }
    a_max_computed = n;
    return a_cache[n-1];
}
