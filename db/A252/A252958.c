/*
 * A252958  Number of (n+2) X (5+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1251, a(2) = 385, a(3) = 605, a(4) = 1027,
 *   a(5) = 1747, a(6) = 3315, a(7) = 6091, a(8) = 11027,
 *   a(9) = 21661, a(10) = 41149, a(11) = 76195, a(12) = 151665,
 *   a(13) = 291595, a(14) = 549597, a(15) = 1095917, a(16) = 2131051
 *   For n > 16:
 *     a(n) =  4*a(n‑1)  – 2*a(n‑2) + 3*a(n‑3) – 37*a(n‑4)
 *            + 20*a(n‑5) + 87*a(n‑6) – 23*a(n‑7) – 8*a(n‑8)
 *            – 423*a(n‑9) + 617*a(n‑10) – 244*a(n‑11) + 70*a(n‑12)
 *            – 105*a(n‑13) + 42*a(n‑14)
 *
 *   The function memoises computed values and can answer calls in any order.
 */
#include <stdlib.h>

/* initial 16 terms */
static const seq_t init_vals[16] = {
    (seq_t)1251,  (seq_t)385,   (seq_t)605,   (seq_t)1027,
    (seq_t)1747,  (seq_t)3315,  (seq_t)6091,  (seq_t)11027,
    (seq_t)21661, (seq_t)41149, (seq_t)76195, (seq_t)151665,
    (seq_t)291595,(seq_t)549597,(seq_t)1095917,(seq_t)2131051
};

/* recurrence coefficients (a(n-1) .. a(n-14)) */
static const int coeff[14] = {
      4, -2,  3, -37,  20,  87, -23,  -8, -423,  617, -244,  70, -105,  42
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1..] holds a(i) */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t  cache_used = 0;    /* highest index computed */

    /* initialise cache if required */
    if (!cache) {
        cache_cap = 17;                      /* 0..16 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init_vals[i-1];
        cache_used = 16;
    }

    if (n <= cache_used)
        return cache[n];

    /* grow cache to hold up to n */
    size_t needed = (size_t)(n + 1);   /* indices 0..n */
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = needed;
    }

    /* compute from cache_used+1 up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t val;
#ifdef __cplusplus
        /* use class operations; assume operator* and -= etc are defined */
        seq_t sum = 0;
        for (int j = 0; j < 14; ++j) {
            if (coeff[j] >= 0)
                sum += (seq_t)(coeff[j]) * cache[i-1-j];
            else
                sum -= (seq_t)(-coeff[j]) * cache[i-1-j];
        }
        val = sum;
#else
        /* use unsigned __int128 for intermediate to avoid overflow in 64‑bit */
        __int128 sum = 0;
        for (int j = 0; j < 14; ++j) {
            sum += (__int128)(coeff[j]) * cache[i-1-j];
        }
        val = (seq_t)sum;
#endif
        cache[i] = val;
    }

    cache_used = n;
    return cache[n];
}
