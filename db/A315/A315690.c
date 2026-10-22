/*
 * A315690  Coordination sequence Gal.5.135.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10, with a(0)=1,
 *            a(1)=6, a(2)=12, a(3)=17, a(4)=22, a(5)=28, a(6)=33, a(7)=38,
 *            a(8)=44, a(9)=50, a(10)=56
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0; /* number of terms stored (cache_len = highest index + 1) */

static const seq_t init_values[11] = {
    (seq_t)1, (seq_t)6, (seq_t)12, (seq_t)17,
    (seq_t)22, (seq_t)28, (seq_t)33, (seq_t)38,
    (seq_t)44, (seq_t)50, (seq_t)56
};

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        /* grow cache to accommodate n */
        idx_t required = n + 1;
        idx_t new_cap = (required > cache_len * 2) ? required : cache_len * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        /* fill missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= (idx_t)10) {
                cache[i] = init_values[i];
            } else {
                cache[i] = cache[i-1] + cache[i-9] - cache[i-10];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
