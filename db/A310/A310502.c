/*
 * A310502  Coordination sequence Gal.5.132.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=20, a(5)=26, a(6)=30,
 *            a(7)=36, a(8)=42, a(9)=46, a(10)=50; a(n) = a(n-1) + a(n-9) -
 *            a(n-10) for n > 10
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t init_vals[11] = {
    (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)16, (seq_t)20,
    (seq_t)26, (seq_t)30, (seq_t)36, (seq_t)42, (seq_t)46,
    (seq_t)50
};

seq_t a(idx_t n)
{
    /* Allocate or grow the cache if necessary */
    if (cache_len <= n) {
        idx_t newlen = n + 1;
        if (newlen < 11) newlen = 11;   /* at least hold the 11 init values */

        if (cache == NULL) {
            cache = (seq_t *)malloc(newlen * sizeof(seq_t));
            assert(cache != NULL);
            /* Initialize the known initial values */
            for (idx_t i = 0; i < 11 && i < newlen; ++i)
                cache[i] = init_vals[i];
            for (idx_t i = 11; i < newlen; ++i)
                cache[i] = (seq_t)0;
            cache_len = newlen;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
            assert(tmp != NULL);
            cache = tmp;
            /* Preserve existing values, initialize new slots */
            for (idx_t i = cache_len; i < newlen; ++i)
                cache[i] = (seq_t)0;
            cache_len = newlen;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i <= 10) continue;  /* already initialized */
        if (cache[i] != 0) continue;  /* already computed */
        cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }

    return cache[n];
}
