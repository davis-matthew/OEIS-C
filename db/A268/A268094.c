/*
 * A268094  Number of 2 X n 0..2 arrays with every repeated value in every row
 *          not one larger and in every column one larger mod 3 than the
 *          previous repeated value, and upper left element zero.
 * https://oeis.org/A268094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 44*a(n-2) - 88*a(n-3) + 352*a(n-4) +
 *            320*a(n-5) - 256*a(n-6); Initial terms: a(1)=3, a(2)=27,
 *            a(3)=243, a(4)=2028, a(5)=16428, a(6)=129792
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t mul_const(seq_t x, unsigned int c) {
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)x * c;
        return (seq_t)tmp;
    } else {
        return x * c;
    }
}

static seq_t *cache = NULL;
static idx_t cached_n = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t idx = (size_t)n;

    /* Allocate / resize cache if necessary. */
    if (!cache || idx + 1 > cached_n + 1) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Base values for n = 1..6. */
    static const seq_t base[7] = {0, 3, 27, 243, 2028, 16428, 129792};
    if (cached_n == 0) {
        size_t to_fill = idx < 6 ? idx : 6;
        for (size_t i = 1; i <= to_fill; ++i) cache[i] = base[i];
        cached_n = (to_fill > 0 ? to_fill : 0);
    }

    /* Compute up to required n. */
    while ((size_t)cached_n < idx) {
        size_t i = cached_n + 1;
        if (i <= 6) {
            cache[i] = base[i];
        } else {
            if (sizeof(seq_t) <= 8) {
                unsigned __int128 pos = (unsigned __int128)cache[i-1]*14
                                      + (unsigned __int128)cache[i-4]*352
                                      + (unsigned __int128)cache[i-5]*320;
                unsigned __int128 neg = (unsigned __int128)cache[i-2]*44
                                      + (unsigned __int128)cache[i-3]*88
                                      + (unsigned __int128)cache[i-6]*256;
                cache[i] = (seq_t)(pos - neg);
            } else {
                seq_t pos = mul_const(cache[i-1],14) + mul_const(cache[i-4],352)
                          + mul_const(cache[i-5],320);
                seq_t neg = mul_const(cache[i-2],44) + mul_const(cache[i-3],88)
                          + mul_const(cache[i-6],256);
                cache[i] = pos - neg;
            }
        }
        cached_n = i;
    }

    return cache[idx];
}
