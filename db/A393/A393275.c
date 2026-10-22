/*
 * A393275  a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) *
 *          binomial(n-1,n-4*k).
 * https://oeis.org/A393275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2*k,k) *
 *            binomial(n-1,n-4*k)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 105,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n < 4)
        return (seq_t)0;

    idx_t   kmax = n / 4;
    seq_t   sum  = 0;
    seq_t   C    = 1;   /* binomial(0,0) */

    for (idx_t k = 1; k <= kmax; ++k) {
        /* C = binomial(2k,k) computed from C_{k-1} */
        C = C * (seq_t)(4 * k - 2) / (seq_t)k;
        idx_t b  = n - 4 * k;            /* n-4k >= 0 */
        seq_t binv = binomial(n - 1, b); /* binomial(n-1, n-4k) */
        sum += C * binv;
    }

    return sum;
}
