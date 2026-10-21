/*
 * A274725  Number of nX5 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) or (-1,0) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A274725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1)+33*a(n-2)+66*a(n-3)+94*a(n-4)
   -115*a(n-5)-155*a(n-6)-255*a(n-7)+236*a(n-8)
   +152*a(n-9)+61*a(n-10)-213*a(n-11)+90*a(n-12)
   -5*a(n-13)+13*a(n-14)-4*a(n-15)   for n > 19
   Initial terms a(1) … a(19) as listed in the OEIS entry.  */

#include <stdlib.h>

static const seq_t init[19] = {
    41ULL, 384ULL, 1300ULL, 10368ULL, 84948ULL,
    687248ULL, 5634776ULL, 45990340ULL, 375845052ULL,
    3070055916ULL, 25083639844ULL, 204917982144ULL,
    1674151403908ULL, 13677241018800ULL, 111739565720152ULL,
    912879351011812ULL, 7457971359536024ULL,
    60929501956856368ULL, 497776886832382136ULL
};

static const int coeffs[15] = {
    3, 33, 66, 94, -115, -155, -255, 236,
    152, 61, -213, 90, -5, 13, -4
};

static seq_t *cache = NULL;   /* cached values a(1)…a(k) */
static idx_t cache_len = 0;   /* number of cached values */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but defined safely */

    /* Return the first 19 terms directly */
    if (n <= 19) return init[n - 1];

    /* Ensure cache contains at least the first 19 terms */
    if (cache_len < 19) {
        size_t alloc = 19;
        cache = (seq_t*)malloc(alloc * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 19; ++i) cache[i] = init[i];
        cache_len = 19;
    }

    /* Grow cache and compute new values up to n */
    if (cache_len < n) {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (idx_t k = 1; k <= 15; ++k) {
                seq_t v   = cache[i - k - 1];          /* a(i-k) */
                int    c  = coeffs[k - 1];
                if (c >= 0)
                    pos += v * (seq_t)c;
                else
                    neg += v * (seq_t)(-c);
            }
            cache[i - 1] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
