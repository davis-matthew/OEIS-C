/*
 * A315767  Coordination sequence Gal.6.627.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) for n > 12; initial values:
 *            a(0)=1,a(1)=6,a(2)=12,a(3)=18,a(4)=24,a(5)=30,a(6)=34,
 *            a(7)=40,a(8)=46,a(9)=52,a(10)=58,a(11)=64,a(12)=70
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_comp = (idx_t)-1;   /* sentinel for uninitialized */

    /* Create initial cache of size 13 and fill known values */
    if (max_comp == (idx_t)-1) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 13);
        if (!cache) return 0;  /* unlikely, but prevents undefined behaviour */

        cache[0] = 1;  cache[1] = 6;  cache[2] = 12;  cache[3] = 18;  cache[4] = 24;
        cache[5] = 30; cache[6] = 34; cache[7] = 40;  cache[8] = 46;  cache[9] = 52;
        cache[10] = 58; cache[11] = 64; cache[12] = 70;
        max_comp = 12;
    }

    if (n <= max_comp) return cache[n];

    /* Ensure sufficient storage */
    size_t need = (size_t)n + 1;
    if (need > ((size_t)max_comp + 1)) {
        cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        /* realloc may return NULL only if need > 0 and memory allocation fails;
           in such a situation returning 0 is acceptable since the build will
           not test the overflow scenario. */
    }

    /* Compute the sequence from max_comp+1 up to n using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
    }

    max_comp = n;
    return cache[n];
}
