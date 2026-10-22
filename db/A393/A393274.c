/*
 * A393274  a(n) = Sum_{k=0..floor(n/3)} binomial(2*k,k) *
 *          binomial(n-1,n-3*k).
 * https://oeis.org/A393274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*k,k) *
 *            binomial(n-1,n-3*k)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binomial(2 * k, k);           /* binomial(2k, k) */
        idx_t m   = n - 3 * k;                    /* n-3k */
        /* binomial(n-1, m) returns 0 if m > n-1 */
        seq_t c2 = binomial(n - 1, m);
        sum += c1 * c2;
    }

    return sum;
}
