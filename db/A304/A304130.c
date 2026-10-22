/*
 * A304130  Number of nX5 0..1 arrays with every element unequal to 0, 1, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +8*a(n-3) -3*a(n-4) -23*a(n-6) -4*a(n-7) -4*a(n-8)
 *            +26*a(n-9) +19*a(n-10) +8*a(n-11) -10*a(n-12) -19*a(n-13)
 *            -4*a(n-14) +6*a(n-16) for n>22 initial values: a(1)=8, a(2)=31,
 *            a(3)=33, a(4)=80, a(5)=176, a(6)=320, a(7)=582, a(8)=1112,
 *            a(9)=2178, a(10)=4222, a(11)=8066, a(12)=15568, a(1...
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 64, u128 n <= 131,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static const seq_t init_vals[23] = {
    0ULL,      /* index 0 unused */
    8ULL,      31ULL, 33ULL,  80ULL, 176ULL, 320ULL,  582ULL,
    1112ULL,   2178ULL, 4222ULL, 8066ULL, 15568ULL, 30220ULL,
    58392ULL, 112902ULL, 218678ULL, 423402ULL, 820096ULL,
    1588910ULL, 3078264ULL, 5965222ULL, 11561350ULL
};

static seq_t *values = NULL;
static idx_t computed = 0;   /* highest n for which values[n] is computed */

static void init_cache(void) {
    values = (seq_t *)calloc(23, sizeof(seq_t));
    for (size_t i = 1; i < 23; ++i)
        values[i] = init_vals[i];
    computed = 22;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (computed == 0) init_cache();
    if (n <= computed) return values[n];

    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(values, new_len * sizeof(seq_t));
    values = tmp;

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = values[i-1];
        pos += mul_small(values[i-3], 8);
        pos += mul_small(values[i-9], 26);
        pos += mul_small(values[i-10], 19);
        pos += mul_small(values[i-11], 8);
        pos += mul_small(values[i-16], 6);

        seq_t neg = mul_small(values[i-4], 3);
        neg += mul_small(values[i-6], 23);
        neg += mul_small(values[i-7], 4);
        neg += mul_small(values[i-8], 4);
        neg += mul_small(values[i-12], 10);
        neg += mul_small(values[i-13], 19);
        neg += mul_small(values[i-14], 4);

        values[i] = pos - neg;   /* pos >= neg, unsigned subtraction is safe */
    }

    computed = n;
    return values[n];
}
