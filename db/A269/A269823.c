/*
 * A269823  Number of n X 2 0..4 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totaling four exactly once.
 * https://oeis.org/A269823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 47*a(n-2) - 336*a(n-3) - 1416*a(n-4) -
 *            864*a(n-5) - 144*a(n-6) (n>7) Initial values: a(1)=5, a(2)=216,
 *            a(3)=3096, a(4)=39784, a(5)=462680, a(6)=5145736, a(7)=55265336
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed = 0;   /* highest index for which cache[i] is set */

    if (n == 0) return 0;        /* offset enforcement: values below offset are irrelevant */

    /* ensure enough memory */
    size_t needed = (size_t)n + 1; /* we ignore index 0 */
    if (needed > cap) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cap   = needed;
    }

    /* initialise the first seven terms once */
    if (computed < 7) {
        static const seq_t init[8] = {
            0ULL,            /* unused, 0-index */
            5ULL,            /* a(1) */
            216ULL,          /* a(2) */
            3096ULL,         /* a(3) */
            39784ULL,        /* a(4) */
            462680ULL,       /* a(5) */
            5145736ULL,      /* a(6) */
            55265336ULL      /* a(7) */
        };
        for (idx_t i = computed + 1; i <= 7 && i <= n; ++i)
            cache[i] = init[(size_t)i];
        computed = 7;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t s1 = cache[i-1] * (seq_t)10 + cache[i-2] * (seq_t)47;
        seq_t s2 = cache[i-3] * (seq_t)336 + cache[i-4] * (seq_t)1416
                 + cache[i-5] * (seq_t)864 + cache[i-6] * (seq_t)144;
        cache[i] = s1 - s2;   /* s1 >= s2 for all n >= 8 */
    }
    computed = n;
    return cache[n];
}
