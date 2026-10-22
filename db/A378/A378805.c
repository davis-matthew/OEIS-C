/*
 * A378805  a(n) = n^2 * 2^n * binomial(4*n, n).
 * https://oeis.org/A378805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * 2^n * binomial(4*n, n) (OEIS A378805)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   binomial_4n_n, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial_4n_n.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t res = (seq_t)n * (seq_t)n;          /* n^2  */
    res *= pow2(n);                          /* * 2^n */
    res *= binomial_4n_n(n);                 /* * binomial(4n, n) */

    return res;
}
