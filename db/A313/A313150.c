/*
 * A313150  Coordination sequence Gal.6.382.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n‑1) - 3*a(n‑2) + 4*a(n‑3) - 5*a(n‑4) + 5*a(n‑5)
   - 4*a(n‑6) + 4*a(n‑7) - 4*a(n‑8) + 4*a(n‑9) - 4*a(n‑10) + 5*a(n‑11)
   - 5*a(n‑12) + 4*a(n‑13) - 3*a(n‑14) + 2*a(n‑15) - a(n‑16)   (n > 19)
   Initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=14, a(4)=20, a(5)=26, a(6)=30,
   a(7)=35, a(8)=41, a(9)=45, a(10)=51, a(11)=57, a(12)=59, a(13)=64,
   a(14)=72, a(15)=75, a(16)=80, a(17)=89, a(18)=91, a(19)=93 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base terms for n = 0..19 */
    static const seq_t base[20] = {
        1, 4, 9, 14, 20, 26, 30, 35, 41, 45,
        51, 57, 59, 64, 72, 75, 80, 89, 91, 93
    };

    /* Recurrence coefficients */
    static const int coeff[16] = {
        2, -3,  4, -5, 5, -4, 4, -4,
        4, -4,  5, -5, 4, -3, 2, -1
    };

    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n < 20)
        return base[(size_t)n];

    /* Initialise cache on first call */
    if (!cache) {
        cache = (seq_t*)malloc(20 * sizeof(seq_t));
        if (!cache)
            abort();
        for (size_t i = 0; i < 20; ++i)
            cache[i] = base[i];
        cache_size = 20;
    }

    /* Extend cache up to n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t total = 0;
            for (int j = 0; j < 16; ++j) {
                seq_t term = cache[i - 1 - j];
                int c = coeff[j];
                if (c > 0)
                    total += term * (seq_t)c;
                else
                    total -= term * (seq_t)(-c);
            }
            cache[i] = total;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
