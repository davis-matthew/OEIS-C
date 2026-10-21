/*
 * A247614  a(n) = Sum_{k=0..9} binomial(18,k)*binomial(n,k).
 * https://oeis.org/A247614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..9} binomial(18,k)*binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 175, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    static const seq_t c[10] = {
        1, 18, 153, 816, 3060, 8568,
        18564, 31824, 43758, 48620
    };
    seq_t sum = 0;
    for (idx_t k = 0; k < 10; ++k) {
        seq_t bk = binomial(n, k);          /* binomial(n,k) */
        if (bk == 0) continue;              /* skip terms where k > n */
        sum += c[k] * bk;                   /* add c[k] * binomial(n,k) */
    }
    return sum;
}
