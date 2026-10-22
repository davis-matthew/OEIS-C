/*
 * A352035  Sum of the 7th powers of the odd proper divisors of n.
 * https://oeis.org/A352035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 49, u64 n <= 1133,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow7(seq_t x) {
    seq_t r = 1;
    for (int i = 0; i < 7; ++i) r *= x;
    return r;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * (idx_t)i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            if ((i & 1) && i != n) {
                sum += pow7((seq_t)i);
            }
            /* paired divisor n / i */
            idx_t j = n / i;
            if (j != i && j != n && (j & 1)) {
                sum += pow7((seq_t)j);
            }
        }
    }
    return sum;
}
