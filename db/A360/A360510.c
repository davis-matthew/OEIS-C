/*
 * A360510  a(n) = Product_{i=2..n} p(i) - p(n+1)^2, where p(i) is the i-th
 *          prime.
 * https://oeis.org/A360510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{i=2..n} p(i) - p(n+1)^2
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A360510. */
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t pi = get_prime(i);
        prod = prod * (seq_t)pi;
    }
    uint64_t pn1 = get_prime(n + 1);
    seq_t sq = (seq_t)pn1 * (seq_t)pn1;
    return prod - sq;
}
