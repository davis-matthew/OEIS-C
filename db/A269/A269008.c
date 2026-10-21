/*
 * A269008  Number of n X 5 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 24*a(n‑1) – 198*a(n‑2) + 584*a(n‑3) + 137*a(n‑4)
   – 2864*a(n‑5) + 1132*a(n‑6) + 4336*a(n‑7) – 1391*a(n‑8) – 2280*a(n‑9)
   + 90*a(n‑10) + 200*a(n‑11) – 25*a(n‑12)   for n > 12
   Initial values: a(1)=10, a(2)=88, a(3)=1078, a(4)=10976,
   a(5)=109058, a(6)=1053432, a(7)=10002542, a(8)=93733440,
   a(9)=869397882, a(10)=7996744280, a(11)=73044076454,
   a(12)=663272676512 */
#include <stdlib.h>
#include <stdbool.h>

/* pre‑computed initial terms a(1) … a(12) – index 0 unused */
static const seq_t init[13] = {
    0ULL,
    (seq_t)10ULL,            (seq_t)88ULL,            (seq_t)1078ULL,
    (seq_t)10976ULL,         (seq_t)109058ULL,        (seq_t)1053432ULL,
    (seq_t)10002542ULL,      (seq_t)93733440ULL,     (seq_t)869397882ULL,
    (seq_t)7996744280ULL,    (seq_t)73044076454ULL,  (seq_t)663272676512ULL
};

/* coefficients of the recurrence – a(n-1) … a(n-12) */
static const int coeff[12] = {
    24,  -198,   584,    137,
   -2864, 1132,  4336,  -1391,
   -2280,   90,   200,   -25
};

seq_t a(idx_t n)
{
    /* cache of all computed values – indices 1 … max_cached */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    if (n <= 12)                     /* return pre‑computed value */
        return init[n];

    /* allocate memory if first use or if we need more space */
    if (!cache) {
        size_t init_cap = (size_t)(12 + 2);                  /* enough for up to index 12 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;   /* out of memory – unlikely in tests */
        max_cached = 12;
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init[i];
    }

    if (n > max_cached) {
        size_t new_cap = (size_t)(n + 2);                /* allow indexing by n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* out of memory – unlikely */
        cache = tmp;
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;                     /* split positive / negative terms */
            for (int k = 1; k <= 12; ++k) {
                int c = coeff[k - 1];
                seq_t term = cache[i - k];
                if (c > 0)
                    pos += (seq_t)c * term;
                else
                    neg += (seq_t)(-c) * term;
            }
            cache[i] = pos - neg;                        /* final value (always non‑negative) */
        }
        max_cached = n;
    }

    return cache[n];
}
