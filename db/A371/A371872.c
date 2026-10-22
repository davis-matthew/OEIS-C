/*
 * A371872  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-2*k-1,n-3*k).
 * https://oeis.org/A371872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-2*k-1, n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* a(0) = 1 */
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t a_val = (idx_t)(2 * n - 2 * k - 1);
        idx_t b_val = (idx_t)(n - 3 * k);
        sum += binomial(a_val, b_val);
    }
    return sum;
}
