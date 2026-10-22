/*
 * A338046  G.f.: Sum_{k>=0} x^(2^k) / (1 - x^(2^k))^4.
 * https://oeis.org/A338046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial_n_plus_2_c3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_n_plus_2_c3.h"
/* 
 * Sequence A338046:
 * a(1) = 1
 * a(n) = C(n+2,3)                         if n is odd
 * a(n) = a(n/2) + C(n+2,3)                if n is even
 * where C(k,3) = k*(k-1)*(k-2)/6.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(0) is unused; a(1) = 1 */
        return (n == 1) ? (seq_t)1 : (seq_t)0;

    if (n & 1)           /* n is odd */
        return binomial_n_plus_2_c3(n);
    else                 /* n is even */
    {
        seq_t val = a(n >> 1);          /* a(n/2) */
        val += binomial_n_plus_2_c3(n); /* C(n+2,3) */
        return val;
    }
}
