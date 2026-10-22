/*
 * A304654  a(n) = (n!)^2 * Sum_{k=1..n-1} 1/(k^2*(n-k)).
 * https://oeis.org/A304654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{k=1..n-1} 1/(k^2*(n-k)) Recurrence (valid
 *            for n ≥ 4): (2*n-3) * a(n) = (6*n^3-25*n^2+33*n-12)*a(n-1) -
 *            (n-2)^2*(6*n^3-29*n^2+42*n-15)*a(n-2) +
 *            (n-3)^3*(n-2)^3*(2*n-1)*a(n-3) Initial values: a(0)=0, a(1)=0,
 *            a(2)=4, a(3)=27
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__SIZEOF_INT128__)
typedef __int128  big_t;        /* signed 128‑bit for intermediate math */
#else
typedef unsigned long long big_t;  /* at least 64‑bit */
#endif

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;
    static idx_t computed   = 0;     /* greatest index already filled */

    /* --- Make sure cache has room for index n -------------------- */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* --- Ensure the first entry exists -------------------------------- */
    if (cache_len == 0) {                 /* first call ever */
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        if (!cache) return 0;
        cache_len = 1;
        cache[0] = 0;
        computed = 0;
    }

    /* --- Compute all missing terms up to n --------------------------- */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 0;
        } else if (i == 2) {
            cache[i] = 4;
        } else if (i == 3) {
            cache[i] = 27;
        } else {
            big_t nb = (big_t)i;
            big_t nb2 = nb * nb;
            big_t nb3 = nb2 * nb;

            big_t coef1 = 6 * nb3 - 25 * nb2 + 33 * nb - 12;

            big_t n_2   = nb - 2;
            big_t coef2 = n_2 * n_2 * (6 * nb3 - 29 * nb2 + 42 * nb - 15);

            big_t n_3   = nb - 3;
            big_t coef3 = n_3 * n_3 * n_3 * n_2 * n_2 * n_2 * (2 * nb - 1);

            big_t term1 = coef1 * (big_t)cache[i - 1];
            big_t term2 = coef2 * (big_t)cache[i - 2];
            big_t term3 = coef3 * (big_t)cache[i - 3];

            big_t numer = term1 - term2 + term3;
            big_t denom = 2 * nb - 3;

            cache[i] = (seq_t)(numer / denom);
        }
    }

    computed = n;
    return cache[n];
}
