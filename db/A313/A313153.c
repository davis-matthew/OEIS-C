/*
 * A313153  Coordination sequence Gal.5.113.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 with initial values
 *            a(0)=1, a(1)=4, a(2)=9, a(3)=14, a(4)=20, a(5)=26, a(6)=32,
 *            a(7)=37, a(8)=42, a(9)=46, a(10)=50
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed values */
    static idx_t  cap     = 0;      /* current allocated capacity */
    static idx_t  computed_n = 10;  /* highest index already computed */
    static int    init    = 0;      /* flag for one‑time initialisation */

    if (!init) {
        cap = 11;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure fallback */
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)9;
        cache[3]  = (seq_t)14;
        cache[4]  = (seq_t)20;
        cache[5]  = (seq_t)26;
        cache[6]  = (seq_t)32;
        cache[7]  = (seq_t)37;
        cache[8]  = (seq_t)42;
        cache[9]  = (seq_t)46;
        cache[10] = (seq_t)50;
        init = 1;
    }

    if (n <= computed_n)
        return cache[n];

    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure fallback */
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = computed_n + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-9) - a(i-10) */
        cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }
    computed_n = n;
    return cache[n];
}
