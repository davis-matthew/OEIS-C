/*
 * A261549  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000101 or 00001011.
 * https://oeis.org/A261549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-2)+3*a(n-3)+a(n-4)-a(n-5)+a(n-6)-2*a(n-7)-2*a(n-8)-3*a(n-9)+2*a(n-10)-a(n-11)-7*a(n-12)+3*a(n-13)-4*a(n-14)-2*a(n-15)+2*a(n-16)
 *            for n>17; Initial values: a(1)=80, a(2)=132, a(3)=269, a(4)=484,
 *            a(5)=871, a(6)=1702, a(7)=3230, a(8)=6084, a(9)=11656,
 *            a(10)=22140, a(11)=41998, a(12)=8016...
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 63, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Coefficients of the recurrence a(n)= Σ coeff[i]*a(n-2-i),  i=0..14  */
static const int coeff[15] = {2, 3, 1, -1, 1, -2, -2, -3, 2, -1, -7, 3, -4, -2, 2};
/* Initial values a(0)  … a(17); index 0 is dummy */
static const seq_t init_vals[18] = {
    0,
    80, 132, 269, 484, 871, 1702, 3230, 6084, 11656, 22140, 41998,
    80163, 152575, 290154, 552862, 1052566, 2003661
};

static seq_t *cache = NULL;   /* points to an array of seq_t, index 0 unused */
static idx_t cache_len = 0;   /* current size of cache (elements 0 … cache_len-1) */

/* Ensure that cache[n] exists and all lower indices are computed. */
static void ensure_cache(idx_t n) {
    if (!cache) {
        /* initial allocation covering indices 0 … 17 */
        size_t init_cap = 18;
#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = calloc(init_cap, sizeof(seq_t));
#endif
        for (idx_t i = 1; i <= 17; ++i) cache[i] = init_vals[i];
        cache_len = init_cap;
    }
    if (n + 1 <= cache_len) return;          /* already available */

    idx_t old_len = cache_len;                 /* keep previous size */
    size_t new_cap = (size_t)(n + 1);          /* new desired capacity */

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
    for (idx_t i = 0; i < old_len; ++i) new_cache[i] = cache[i];
    delete[] cache;
#else
    new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_len = new_cap;

    /* compute newly needed values */
    for (idx_t i = old_len; i <= n; ++i) {
        if (i <= 17) {                 /* initial values already set */
            cache[i] = init_vals[i];
            continue;
        }
        seq_t pos = 0, neg = 0;
        for (int j = 0; j < 15; ++j) { /* apply recurrence */
            idx_t idx = i - (2 + j);
            seq_t term = cache[idx];
            int c = coeff[j];
            if (c > 0) {
                pos += term * (seq_t)c;
            } else {
                neg += term * (seq_t)(-c);
            }
        }
        cache[i] = pos - neg;
    }
}

/* External function computing the sequence. */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
