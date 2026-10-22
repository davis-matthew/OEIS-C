/*
 * A315964  Coordination sequence Gal.6.528.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6) +
 *            a(n-7) + a(n-8) - a(n-9) + a(n-10) - a(n-11) + a(n-12) - a(n-13)
 *            + a(n-14) - a(n-15) (for n > 15) Initial values: a(0)=1, a(1)=6,
 *            a(2)=9, a(3)=13, a(4)=18, a(5)=21, a(6)=26, a(7)=29, a(8)=36,
 *            a(9)=41, a(10)=43, a(11)=48, a(12)=53, a(13)...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 16 known initial terms (indices 0..15) */
    static const seq_t init[16] = {
        1, 6, 9, 13, 18, 21, 26, 29,
        36, 41, 43, 48, 53, 56, 61, 64
    };

    /* Coefficients for the linear recurrence: +1,-1,+1,-1,+1,-1,+1,+1,-1,+1,-1,+1,-1,+1,-1 */
    static const int coeff[15] = {
        +1, -1, +1, -1, +1, -1, +1, +1, -1, +1, -1, +1, -1, +1, -1
    };

    /* Static storage for memoisation */
    static seq_t *cache   = NULL;   /* array of calculated terms */
    static idx_t  size    = 0;      /* current allocated capacity */
    static idx_t  cur_max = 0;      /* highest index that has been computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        size    = 16;             /* capacity sufficient for the 16 known terms */
        cur_max = 15;             /* last filled index */
        cache   = (seq_t *)malloc(size * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        for (idx_t i = 0; i < size; ++i) cache[i] = init[i];
    }

    /* If requested index is already computed, return it */
    if (n <= cur_max) return cache[n];

    /* Grow the cache if needed */
    while (size <= n) {
        idx_t new_size = size * 2 + 2;   /* power‑of‑two growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;       /* allocation failure */
        cache = tmp;
        size  = new_size;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 15; ++k) {
            seq_t term = cache[i - k];
            if (coeff[k - 1] > 0) pos += term;
            else                  neg += term;
        }
        cache[i] = pos - neg;  /* correctly handles unsigned arithmetic */
    }

    cur_max = n;
    return cache[n];
}
