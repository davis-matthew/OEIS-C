/*
 * A251395  Number of (n+1)X(1+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A251395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 30*a(n-2) + 69*a(n-3) - 62*a(n-4) + 30*a(n-5)
 *            - 8*a(n-6) + a(n-7); initial values: a(1)=226, a(2)=3205,
 *            a(3)=45542, a(4)=647154, a(5)=9195809, a(6)=130668665,
 *            a(7)=1856748750
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc/free */

static seq_t *seq_cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static const seq_t init_vals[7] = {
    (seq_t)226ull,
    (seq_t)3205ull,
    (seq_t)45542ull,
    (seq_t)647154ull,
    (seq_t)9195809ull,
    (seq_t)130668665ull,
    (seq_t)1856748750ull
};

static void ensure_cache_cap(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = needed > 64 ? needed : 64;
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
    if (seq_cache) {
#ifdef __cplusplus
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];
        delete [] seq_cache;
#else
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = seq_cache[i];
        free(seq_cache);
#endif
    }
    seq_cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined, but return 0 */

    if (n <= 7)
        return init_vals[n - 1];

    ensure_cache_cap(n);

    if (cache_len < 7) {
        for (idx_t i = 0; i < 7; ++i)
            seq_cache[i] = init_vals[i];
        cache_len = 7;
    }

    for (idx_t i = cache_len; i < n; ++i) {
        /* i is the index of the element to compute (0‑based); we compute a(i+1) */
        seq_t term = seq_cache[i - 1] * (seq_t)16;
        term -= seq_cache[i - 2] * (seq_t)30;
        term += seq_cache[i - 3] * (seq_t)69;
        term -= seq_cache[i - 4] * (seq_t)62;
        term += seq_cache[i - 5] * (seq_t)30;
        term -= seq_cache[i - 6] * (seq_t)8;
        term += seq_cache[i - 7];
        seq_cache[i] = term;
    }
    cache_len = n;
    return seq_cache[n - 1];
}
