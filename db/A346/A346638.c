/*
 * A346638  a(n) is the number of 6-tuples (a_1,a_2,a_3,a_4,a_5,a_6) having
 *          all terms in {1,...,n} such that there exists a hexagon with these
 *          side-lengths.
 * https://oeis.org/A346638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - 6*binomial(n+1,6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t nse = (seq_t)n;
    seq_t pow6 = nse;
    for (idx_t i = 1; i < 6; ++i) {
        pow6 = pow6 * nse;
    }
    if (n < 5) {
        return pow6;
    }
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= 6; ++i) {
        seq_t num = (seq_t)(n + 2 - i);
        binom = binom * num;
        binom = binom / i;
    }
    seq_t sub = binom * (seq_t)6;
    return pow6 - sub;
}
