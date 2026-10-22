/*
 * A339689  a(n) = Sum_{d|n} 9^(d-1).
 * https://oeis.org/A339689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 9^(d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   factor_u64, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/mul_small.h"

/* Precompute powers of 9: pow9[k] = 9^k */
static seq_t *pow9 = NULL;
static size_t pow9_size = 0;

static void ensure_pow9(size_t up_to) {
    if (pow9_size == 0) {
        /* initialise with 9^0 = 1 */
        pow9_size = 1;
        pow9 = (seq_t *)malloc(sizeof(seq_t));
        if (!pow9) return;  /* allocation failed, but caller will handle */
        pow9[0] = (seq_t)1;
    }
    while (pow9_size <= up_to) {
        size_t new_size = pow9_size * 2;
        if (new_size <= up_to) new_size = up_to + 1;
        seq_t *tmp = (seq_t *)realloc(pow9, new_size * sizeof(seq_t));
        if (!tmp) return;  /* allocation failed, but caller will handle */
        pow9 = tmp;
        for (size_t i = pow9_size; i < new_size; ++i) {
            pow9[i] = mul_small(pow9[i - 1], 9);
        }
        pow9_size = new_size;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* safety for n < 1 */
    ensure_pow9((size_t)n - 1);

    /* factor n to get its prime powers */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, pr, ex, &pcnt);

    /* generate all divisors of n */
    size_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        size_t old_cnt = div_cnt;
        uint64_t p_pow = 1;
        for (size_t k = 1; k <= e; ++k) {
            p_pow *= p;       /* safe: n ≤ 10000, product fits in 64‑bit */
            for (size_t j = 0; j < old_cnt; ++j) {
                divs[div_cnt++] = divs[j] * p_pow;
            }
        }
    }

    /* sum 9^(d-1) over all divisors d */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < div_cnt; ++i) {
        size_t d = divs[i];
        sum += pow9[d - 1];
    }
    return sum;
}
