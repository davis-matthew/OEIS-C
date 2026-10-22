/*
 * A300348  Number of nX6 0..1 arrays with every element equal to 1 or 2
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3) + a(n-7) - 3*a(n-8) +
 *            4*a(n-9) - 2*a(n-10) + a(n-11) - a(n-12) - a(n-16) + a(n-17)
 *            (empirical recurrence for n>22) Initial values: a(1)=5, a(2)=90,
 *            a(3)=133, a(4)=325, a(5)=729, a(6)=1371, a(7)=3092, a(8)=6068,
 *            a(9)=12638, a(10)=26306, a(11)=55227, a(12)=1161...
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 26, u64 n <= 55, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[25] = {
    0ull,
    5ull,   90ull,  133ull,  325ull,  729ull,
    1371ull,3092ull,6068ull,12638ull,26306ull,
    55227ull,116124ull,244628ull,515923ull,1089092ull,
    2300375ull,4859824ull,10269075ull,21701677ull,45866023ull,
    96942392ull,204906286ull,433123097ull,915543514ull
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest index currently stored */

    if (n == 0) return 0;               /* not used but harmless */

    /* initialise cache with known values once */
    if (cache == NULL) {
        cache_size = 24;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure – unlikely */
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init_vals[i];
    }

    /* value already computed? */
    if (n <= cache_size) return cache[n];

    /* grow cache to accommodate n */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;              /* allocation failure */
    cache = new_cache;

    /* compute new terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 24) {                     /* already filled during init */
            /* nothing to do */
        } else {
            /* positive contributions */
            seq_t pos = 0;
            pos += cache[i-1] * 5;
            pos += cache[i-3] * 4;
            pos += cache[i-7];
            pos += cache[i-9] * 4;
            pos += cache[i-11];
            pos += cache[i-17];

            /* negative contributions */
            seq_t neg = 0;
            neg += cache[i-2] * 8;
            neg += cache[i-8] * 3;
            neg += cache[i-10] * 2;
            neg += cache[i-12];
            neg += cache[i-16];

            cache[i] = pos - neg;
        }
    }
    cache_size = n;
    return cache[n];
}
