/*
 * A320495  Coordination sequence of thinnest 5-neighbor packing of the plane
 *          with congruent hexagons with respect to a point of type A.
 * https://oeis.org/A320495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence sum_{k=0}^{20} g_k * a(n-k) = f_n,
 *            where g_0 = 1, g = (1-x²)(1-x⁶)(1+x⁴)(1+x⁸) and f(x) =
 *            1+4x+5x²+8x³+9x⁴+11x⁵+12x⁶+13x⁷+14x⁸+9x⁹+11x¹⁰
 *            +16x¹¹+13x¹²+8x¹³+9x¹⁴+16x¹⁵+13x¹⁶+5x¹⁷+4x¹⁸+6x¹⁹+5x²⁰
 *            -x²¹-x²²+x²³-2x²⁴
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#  define CACHE_ALLOC(n)  new seq_t[(n)]
#  define CACHE_FREE(p)   delete[] (p)
#  define CACHE_COPY(dst, src, cnt) \
    for (size_t i = 0; i < (cnt); ++i) (dst)[i] = (src)[i]
#else
#  define CACHE_ALLOC(n)  ((seq_t*)malloc((n)*sizeof(seq_t)))
#  define CACHE_FREE(p)   free(p)
#  define CACHE_COPY(dst, src, cnt) \
    memcpy((dst), (src), (cnt)*sizeof(seq_t))
#endif

/* Denominator coefficients g_0 … g_20 */
static const int8_t g_coeff[21] = {
    1,  0, -1, 0,  1, 0, -2, 0,
    2, 0, -2, 0,  2, 0, -2, 0,
    1, 0, -1, 0, 1
};

/* Numerator coefficients f_0 … f_24 */
static const int16_t f_coeff[25] = {
    1,  4,  5,  8,  9, 11, 12, 13, 14,  9,
   11, 16, 13,  8,  9, 16, 13,  5,  4,  6,
    5, -1, -1,  1, -2
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* initial cache allocation */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = CACHE_ALLOC(cache_cap);
        cache_len = 0;
    }

    /* grow cache if needed (size >= n+1) */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = CACHE_ALLOC(new_cap);
        CACHE_COPY(new_cache, cache, cache_len);
        CACHE_FREE(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (cache_len <= (size_t)n) {
        size_t i = cache_len;

        /* compute negative and positive weighted sums */
        seq_t pos = 0, neg = 0;
        for (int k = 1; k <= 20; ++k) {
            if (i < (size_t)k) continue;
            seq_t term = cache[i - k];
            int8_t gk = g_coeff[k];
            if (gk > 0)
                pos += term * (seq_t)gk;
            else if (gk < 0)
                neg += term * (seq_t)(-gk);
        }

        /* start result with negative-sum part and add f_i */
        seq_t res = neg;
        if (i <= 24) {
            int16_t ft = f_coeff[i];
            if (ft >= 0)
                res += (seq_t)ft;
            else
                res -= (seq_t)(-ft);
        }

        /* subtract positive-sum part */
        res -= pos;

        cache[i] = res;
        ++cache_len;
    }

    return cache[(size_t)n];
}
