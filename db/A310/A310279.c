/*
 * A310279  Coordination sequence Gal.4.2.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) – a(n-2) + a(n-3) + a(n-7) – a(n-8) + a(n-9) –
 *            a(n-10) with initial values: a(0)=1, a(1)=3, a(2)=8, a(3)=11,
 *            a(4)=12, a(5)=19, a(6)=24, a(7)=25, a(8)=26, a(9)=37, a(10)=40,
 *            a(11)=36, a(12)=44, a(13)=52, a(14)=53, a(15)=50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for 0 <= i < cap */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t size  = 0;   /* index of last computed value + 1 */

    /* initial values for indices 0..15 */
    static const seq_t init[16] = {
        (seq_t)1,  (seq_t)3,  (seq_t)8,  (seq_t)11, (seq_t)12,
        (seq_t)19, (seq_t)24, (seq_t)25, (seq_t)26, (seq_t)37,
        (seq_t)40, (seq_t)36, (seq_t)44, (seq_t)52, (seq_t)53, (seq_t)50
    };

    /* allocate or enlarge cache on first request */
    if (!cache) {
        /* start with at least capacity 16 */
        cap = n + 1;
        if (cap < 16) cap = 16;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i) cache[i] = init[i];
        size = 16;
    } else if (n + 1 > cap) {        /* need more space */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 as safe default */
        cache = tmp;
        cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t k = size; k <= n; ++k) {
        if (k <= 15) {
            /* already initialized in init array */
            continue;
        }
        /* recurrence: a(k) = a(k-1) - a(k-2) + a(k-3) + a(k-7)
                           - a(k-8) + a(k-9) - a(k-10)   */
        seq_t val = cache[k-1];
        val -= cache[k-2];
        val += cache[k-3];
        val += cache[k-7];
        val -= cache[k-8];
        val += cache[k-9];
        val -= cache[k-10];
        cache[k] = val;
    }
    size = n + 1;
    return cache[n];
}
