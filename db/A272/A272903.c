/*
 * A272903  Least nonnegative integer x such that n^2+nx-2n-x is prime.
 * https://oeis.org/A272903
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least nonnegative integer x such that n^2 + n*x - 2*n - x
 *            is prime. Offset: 2 (so a(2) = 2)
 * Range:     u8 n <= 16, u16 n <= 795, u32 n <= 705, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Undefined for n < 2; return 0 but the offset ensures n >= 2 in practice */
    if (n < 2) return 0;

    seq_t base  = (seq_t)n * ((seq_t)n - 2); /* n*(n-2) */
    seq_t coef  = (seq_t)n - 1;               /* n-1   */
    idx_t x = 0;

    for (;;) {
        seq_t v = base + (seq_t)x * coef;    /* n^2 + n*x - 2*n - x */
        if (is_prime_big(v)) return (seq_t)x;
        ++x;
    }
}
