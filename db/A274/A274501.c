/*
 * A274501  a(n) = 25*binomial(n-1,6) + binomial(n-1,5).
 * https://oeis.org/A274501
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*binomial(n-1,6) + binomial(n-1,5)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_basic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_basic.h"

seq_t a(idx_t n) {
    if (n < (idx_t)5) return (seq_t)0;   /* below offset, value irrelevant */
    idx_t m = n - (idx_t)1;               /* use n-1 in the formulas */

    seq_t c5 = binom_basic(m, (idx_t)5);
    seq_t c6 = binom_basic(m, (idx_t)6);

    return (seq_t)25 * c6 + c5;
}
