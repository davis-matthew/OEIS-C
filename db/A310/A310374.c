/*
 * A310374  Coordination sequence Gal.6.245.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) a(0) = 1, a(1) = 4, a(2) = 10,
 *            a(3) = 14, a(4) = 18, a(5) = 23, a(6) = 27, a(7) = 32, a(8) =
 *            36, a(9) = 40, a(10) = 46, a(11) = 50, a(12) = 54
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values */
    static idx_t cap   = 0;          /* allocated capacity */
    static idx_t len   = 0;          /* number of valid entries (len-1 is last filled) */

    /* Grow cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap << 1 : 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Fill missing values */
    if (len <= n) {
        static const seq_t init[13] = {
            (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)14, (seq_t)18,
            (seq_t)23, (seq_t)27, (seq_t)32, (seq_t)36, (seq_t)40,
            (seq_t)46, (seq_t)50, (seq_t)54
        };
        for (idx_t i = len; i <= n; ++i) {
            if (i <= 12) {
                cache[i] = init[i];
            } else {
                cache[i] = cache[i-1] + cache[i-11] - cache[i-12];
            }
        }
        len = n + 1;
    }

    return cache[n];
}
