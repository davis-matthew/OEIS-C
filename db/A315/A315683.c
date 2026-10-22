/*
 * A315683  Coordination sequence Gal.6.634.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=12, a(3)=17, a(4)=22, a(5)=27, a(6)=32,
 *            a(7)=37, a(8)=42, a(9)=47, a(10)=52. For n>10: a(n) = 2*a(n-1) -
 *            a(n-2) - a(n-4) + 2*a(n-5) - a(n-6) - a(n-8) + 2*a(n-9) -
 *            a(n-10) The sequence is computed on demand and memoised across
 *            calls
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[11] = {
    (seq_t)1,  (seq_t)6,  (seq_t)12, (seq_t)17, (seq_t)22,
    (seq_t)27, (seq_t)32, (seq_t)37, (seq_t)42, (seq_t)47,
    (seq_t)52
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n < cache_size) return cache[(size_t)n];

    /* ensure cache has at least n+1 entries */
    idx_t needed = n + 1;
    if (needed > cache_size) {
        seq_t *old = cache;
        cache = (seq_t *)realloc(old, (size_t)needed * sizeof(seq_t));
        if (!cache) abort();               /* allocation failure */
        /* initialise new slots from previous size up to needed-1 */
        for (idx_t i = cache_size; i < needed; ++i) {
            if (i <= 10) {
                cache[(size_t)i] = init_vals[(size_t)i];
            } else {
                /* recurrence uses 1-based indices relative to 0 */
                seq_t t = cache[(size_t)(i-1)] << 1;  /* 2*a(i-1) */
                t -= cache[(size_t)(i-2)];           /* -a(i-2) */
                t -= cache[(size_t)(i-4)];           /* -a(i-4) */
                t += (cache[(size_t)(i-5)] << 1);    /* +2*a(i-5) */
                t -= cache[(size_t)(i-6)];           /* -a(i-6) */
                t -= cache[(size_t)(i-8)];           /* -a(i-8) */
                t += (cache[(size_t)(i-9)] << 1);    /* +2*a(i-9) */
                t -= cache[(size_t)(i-10)];          /* -a(i-10) */
                cache[(size_t)i] = t;
            }
        }
        cache_size = needed;
    }

    return cache[(size_t)n];
}
