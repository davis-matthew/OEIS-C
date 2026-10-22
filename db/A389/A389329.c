/*
 * A389329  a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) *
 *          binomial(n+k-1,n-3*k).
 * https://oeis.org/A389329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) *
 *            binomial(n+k-1,n-3*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0, where binomial(n+k-1,…) would use a
       negative index.  For n > 0 the formula works with non‑negative
       arguments. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t kmax = n / 3;          /* floor(n/3) */
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t t1 = binomial(n, k);
        seq_t t2 = binomial(n + k - 1, n - 3 * k);
        sum += t1 * t2;
    }
    return sum;
}
