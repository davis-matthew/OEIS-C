/*
 * A348144  a(n) = Sum_{d|n} n^(n^2/d).
 * https://oeis.org/A348144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^(n^2/d)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle trivial index */
    if (n == 0) return (seq_t)0;

    /* Compute n^2 once */
    seq_t nsq = (seq_t)n * (seq_t)n;

    seq_t total = (seq_t)0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t exp1 = nsq / (seq_t)d1;
            seq_t term1 = big_pow((seq_t)n, exp1);
            total += term1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t exp2 = nsq / (seq_t)d2;
                seq_t term2 = big_pow((seq_t)n, exp2);
                total += term2;
            }
        }
    }
    return total;
}
