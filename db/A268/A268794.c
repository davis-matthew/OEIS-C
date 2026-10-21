/*
 * A268794  Number of nX4 0..2 arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A268794
 * Recurrence for n > 20:
 *   a(n) = 2*a(n‑1) +19*a(n‑2) +10*a(n‑3)
 *          −122*a(n‑4) −320*a(n‑5) −295*a(n‑6)
 *          +8*a(n‑7) +176*a(n‑8) +20*a(n‑9)
 *          −98*a(n‑10) −6*a(n‑11) +43*a(n‑12)
 *          −6*a(n‑13) −11*a(n‑14) +6*a(n‑15) −a(n‑16)
 * Initial values (1‑based):
 *   a(1)=36, a(2)=234, a(3)=950, a(4)=4800,
 *   a(5)=23994, a(6)=117062, a(7)=561116, a(8)=2652936,
 *   a(9)=12405748, a(10)=57490444, a(11)=264428454,
 *   a(12)=1208522850, a(13)=5493284922, a(14)=24851601802,
 *   a(15)=111963916212, a(16)=502590509990, a(17)=2248738675490,
 *   a(18)=10032298715910, a(19)=44640056867750,
 *   a(20)=198161794087570
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* 1‑based initial values, a[0] unused */
static const seq_t init_values[21] = {
    0ULL, /* placeholder for index 0 */
    36ULL, 234ULL, 950ULL, 4800ULL,
    23994ULL, 117062ULL, 561116ULL, 2652936ULL, 12405748ULL,
    57490444ULL, 264428454ULL, 1208522850ULL, 5493284922ULL,
    24851601802ULL, 111963916212ULL, 502590509990ULL,
    2248738675490ULL, 10032298715910ULL,
    44640056867750ULL, 198161794087570ULL
};

/* coefficients for a(n‑1) … a(n‑16) */
static const int coeff[16] = {
    2, 19, 10, -122, -320, -295, 8, 176,
    20, -98, -6, 43, -6, -11, 6, -1
};

/* Dynamic cache of computed values */
static seq_t *cache = NULL;          /* array of a(i) */
static idx_t  cache_size = 0;        /* allocated size (>= max_computed+1) */
static idx_t  max_computed = 0;      /* highest n for which cache[n] is valid */

/* Grow the cache to at least [0..n] */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        cache_size = n + 1;
        if (cache_size < 21) cache_size = 21;          /* store all initial values */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        memset(cache, 0, cache_size * sizeof(seq_t));
    }
    else if (cache_size <= n) {
        idx_t new_size = (n + 1 > 2 * cache_size) ? (n + 1) : (2 * cache_size);
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        memcpy(new_cache, cache, (max_computed + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range: 0 */

    /* Return known initial values directly */
    if (n <= 20) return init_values[n];

    /* Prepare cache */
    ensure_cache_size(n);

    /* Load initial values once */
    if (max_computed < 20) {
        for (idx_t i = 1; i <= 20; ++i) cache[i] = init_values[i];
        max_computed = 20;
    }

    /* Compute successive terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos_sum = 0, neg_sum = 0;
        for (int k = 0; k < 16; ++k) {
            idx_t j = i - (k + 1);
            seq_t prev = cache[j];
            int coef = coeff[k];
            if (coef > 0) {
                pos_sum += prev * (seq_t)coef;
            } else {
                neg_sum += prev * (seq_t)(-coef);
            }
        }
        cache[i] = pos_sum - neg_sum;
    }
    max_computed = n;
    return cache[n];
}
