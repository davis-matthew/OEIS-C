/*
 * A383868  a(n) = 2^(n-3)*(3*binomial(n,4) + 4*binomial(n,2) + 8).
 * https://oeis.org/A383868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3)*(3*binomial(n,4) + 4*binomial(n,2) + 8)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    seq_t bin4 = binomial(n, 4);
    seq_t bin2 = binomial(n, 2);
    seq_t term = bin4 * 3 + bin2 * 4 + 8;
    return pow2(n - 3) * term;
}
