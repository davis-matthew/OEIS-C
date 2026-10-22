/*
 * A347017  a(n) = floor(2^(n-1)) - binomial(n,3) + binomial(n,2) - n + 1.
 * https://oeis.org/A347017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) – binomial(n,3) + binomial(n,2) – n + 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t term = pow2(n - 1);          // 2^(n-1)
    term = term - binomial(n, 3);      // subtract binomial(n,3)
    term = term + binomial(n, 2);      // add binomial(n,2)
    term = term - n + 1;               // subtract n, add 1
    return term;
}
