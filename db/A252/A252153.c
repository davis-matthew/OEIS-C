/*
 * A252153  Number of (n+2)X(4+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum not equal to 0 1 3 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum equal to 0 1 3 6 or 7.
 * https://oeis.org/A252153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A252153 a(n) = 3*a(n-1) -2*a(n-2) +6*a(n-3) -18*a(n-4)
 *            +12*a(n-5) -8*a(n-6) +24*a(n-7) -16*a(n-8) for n>13 with initial
 *            values a(1)=3583, a(2)=2702, a(3)=4988, a(4)=8425, a(5)=15460,
 *            a(6)=29825, a(7)=58792, a(8)=116276, a(9)=229960, a(10)=457196,
 *            a(11)=909868, a(12)=1810124, a(13)=3610156
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_vals[14] = {
    0ULL,          /* 0 unused */
    3583ULL,       /* 1 */
    2702ULL,       /* 2 */
    4988ULL,       /* 3 */
    8425ULL,       /* 4 */
    15460ULL,      /* 5 */
    29825ULL,      /* 6 */
    58792ULL,      /* 7 */
    116276ULL,     /* 8 */
    229960ULL,     /* 9 */
    457196ULL,     /* 10 */
    909868ULL,     /* 11 */
    1810124ULL,    /* 12 */
    3610156ULL     /* 13 */
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t highest_computed = 0;

seq_t a(idx_t n)
{
    /* allocation and base initialization */
    if (cache == NULL) {
        cache_len = 14; /* indices 0..13 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < cache_len; ++i) cache[i] = 0;
        for (size_t i = 1; i <= 13; ++i) cache[i] = base_vals[i];
        highest_computed = 13;
    }

    /* grow cache if necessary */
    size_t needed_len = (size_t)n + 1;
    if (needed_len > cache_len) {
        size_t new_len = needed_len;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        /* initialize newly allocated part to 0 */
        for (size_t i = cache_len; i < new_len; ++i) tmp[i] = 0;
        cache = tmp;
        cache_len = new_len;
    }

    if (n <= highest_computed) return cache[(size_t)n];

    /* recurrence coefficients */
    const int coeffs[8] = {3, -2, 6, -18, 12, -8, 24, -16};

    /* compute values up to n */
    for (size_t i = highest_computed + 1; i <= (size_t)n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 8; ++k) {
            size_t idx = i - (k + 1);
            int c = coeffs[k];
            if (c > 0)
                pos += (seq_t)c * cache[idx];
            else
                neg += (seq_t)(-c) * cache[idx];
        }
        cache[i] = pos - neg; /* safe: pos >= neg by recurrence */
    }

    highest_computed = (size_t)n;
    return cache[(size_t)n];
}
