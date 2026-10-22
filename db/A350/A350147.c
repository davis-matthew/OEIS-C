/*
 * A350147  a(n) = Sum_{k=1..n} floor(n/(2*k-1))^k.
 * https://oeis.org/A350147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/(2*k-1))^k
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    /* For k > (n+1)/2 the divisor 2*k-1 exceeds n, so the floor is 0. */
    idx_t limit = (n + 1) / 2;
    for (idx_t k = 1; k <= limit; ++k) {
        idx_t denom = 2 * k - 1;
        seq_t base = (seq_t)(n / denom);
        /* base is at least 1 here */
        seq_t term = big_pow(base, (seq_t)k);
        sum += term;
    }
    return sum;
}
