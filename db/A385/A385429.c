/*
 * A385429  Number of triples (di, dj, dk) of divisors of 2*n such that di+dj
 *          = dk.
 * https://oeis.org/A385429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search → Count ordered pairs of divisors of 2*n whose sum is also
   a divisor of 2*n (each such pair gives one triple (di, dj, dk)). */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t M = n * 2;  /* 2*n */
    
    /* Dynamic array for divisors of M */
    size_t div_cap = 8;
    idx_t *divs = (idx_t *)malloc(div_cap * sizeof(idx_t));
    size_t div_cnt = 0;
    
    /* Find all divisors by examining numbers up to sqrt(M) */
    for (idx_t d = 1; d <= M / d; ++d) {
        if (M % d == 0) {
            if (div_cnt == div_cap) {
                div_cap *= 2;
                divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
            }
            divs[div_cnt++] = d;
            idx_t d2 = M / d;
            if (d2 != d) {
                if (div_cnt == div_cap) {
                    div_cap *= 2;
                    divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                }
                divs[div_cnt++] = d2;
            }
        }
    }
    
    /* Count ordered pairs whose sum also divides M */
    seq_t count = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        for (size_t j = 0; j < div_cnt; ++j) {
            /* avoid overflow in sum: only consider sums <= M */
            if (divs[i] > M - divs[j]) continue;
            idx_t sum = divs[i] + divs[j];  /* now guaranteed sum <= M */
            if (M % sum == 0)
                ++count;
        }
    }
    
    free(divs);
    return count;
}
