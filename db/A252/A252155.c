/*
 * A252155  Number of (n+2)X(6+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 0 1 3 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum equal to 0 1 3 6 or 7.
 * https://oeis.org/A252155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +14*a(n-3) -14*a(n-4) -56*a(n-6) +56*a(n-7)
 *            +64*a(n-9) -64*a(n-10) for n > 15 a(1) = 8309, a(2) = 8769, a(3)
 *            = 17803, a(4) = 29825, a(5) = 57271, a(6) = 112547, a(7) =
 *            223442, a(8) = 447713, a(9) = 891380, a(10) = 1776728, a(11) =
 *            3561844, a(12) = 7105520, a(13) = 14181200, a(14) =...
 * Range:     u8 none, u16 n <= 5, u32 n <= 21, u64 n <= 53, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[15] = {
    (seq_t)8309,      (seq_t)8769,      (seq_t)17803,
    (seq_t)29825,     (seq_t)57271,     (seq_t)112547,
    (seq_t)223442,    (seq_t)447713,    (seq_t)891380,
    (seq_t)1776728,   (seq_t)3561844,   (seq_t)7105520,
    (seq_t)14181200,  (seq_t)28426208,  (seq_t)56753264
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of terms stored (≥ 15 after init) */
    static idx_t cache_cap = 0;

    if (n == 0) return 0; /* undefined, but harmless */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* memory allocation failure */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init_terms[i];
        cache_len = 15;
    }

    if (n <= cache_len)
        return cache[n - 1];

    /* grow cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cache_cap = cache_len = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t sum = cache[i - 2];                   /* a(i-1)   */
        sum += (seq_t)14 * cache[i - 4];            /* +14*a(i-3) */
        sum += (seq_t)56 * cache[i - 8];            /* +56*a(i-7) */
        sum += (seq_t)64 * cache[i - 10];           /* +64*a(i-9) */

        seq_t sub = (seq_t)14 * cache[i - 5];        /* -14*a(i-4) */
        sub += (seq_t)56 * cache[i - 7];            /* -56*a(i-6) */
        sub += (seq_t)64 * cache[i - 11];           /* -64*a(i-10) */

        cache[i - 1] = sum - sub;
    }

    cache_len = n;
    return cache[n - 1];
}
