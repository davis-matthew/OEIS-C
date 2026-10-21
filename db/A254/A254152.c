/*
 * A254152  Number of independent sets in the generalized Aztec diamond
 *          E(L_9,L_{2n-1}).
 * https://oeis.org/A254152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 74*a(n-1) - 1450*a(n-2) + 10672*a(n-3) - 34214*a(n-4) +
 *            50814*a(n-5) - 34671*a(n-6) + 9772*a(n-7) - 936*a(n-8) Initial
 *            values: a(0)=1, a(1)=32, a(2)=1351, a(3)=62501, a(4)=2976416,
 *            a(5)=142999897, a(6)=6888568813, a(7)=332097693792
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed = (idx_t)-1;   /* sentinel meaning nothing computed yet */

    /* initial values (only 8 needed for the recurrence) */
    static const seq_t base[] = {
        (seq_t)1,               /* a(0) */
        (seq_t)32,              /* a(1) */
        (seq_t)1351,            /* a(2) */
        (seq_t)62501,           /* a(3) */
        (seq_t)2976416,         /* a(4) */
        (seq_t)142999897,       /* a(5) */
        (seq_t)6888568813,      /* a(6) */
        (seq_t)332097693792     /* a(7) */
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();   /* fail fast */
        for (size_t i = 0; i < 8; ++i) cache[i] = base[i];
        computed = 7;          /* we already have a(0)..a(7) */
    }

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size;
        while ((size_t)(n + 1) > new_size) new_size *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* if result already computed, return it */
    if (n <= computed) return cache[n];

    /* recurrence coefficients (positive for sum_pos, negative for sum_neg) */
    static const int coeff_pos[4] = {74, 10672, 50814, 9772};
    static const int coeff_neg[4] = {1450, 34214, 34671, 936};
    static const idx_t pos_idx[4] = {1, 3, 5, 7};
    static const idx_t neg_idx[4] = {2, 4, 6, 8};

    /* compute values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* handle positive coefficients */
        sum_pos += (seq_t)coeff_pos[0] * cache[i - pos_idx[0]];
        sum_pos += (seq_t)coeff_pos[1] * cache[i - pos_idx[1]];
        sum_pos += (seq_t)coeff_pos[2] * cache[i - pos_idx[2]];
        sum_pos += (seq_t)coeff_pos[3] * cache[i - pos_idx[3]];

        /* handle negative coefficients */
        sum_neg += (seq_t)coeff_neg[0] * cache[i - neg_idx[0]];
        sum_neg += (seq_t)coeff_neg[1] * cache[i - neg_idx[1]];
        sum_neg += (seq_t)coeff_neg[2] * cache[i - neg_idx[2]];
        sum_neg += (seq_t)coeff_neg[3] * cache[i - neg_idx[3]];

        /* a(i) = sum_pos - sum_neg  (sum_pos >= sum_neg for all valid i) */
        cache[i] = sum_pos - sum_neg;
    }
    computed = n;
    return cache[n];
}
