/*
 * A387508  a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(n-3*k,k)^2.
 * https://oeis.org/A387508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 2^k * binomial(n-3*k,k)^2
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 62, u128 n <= 122,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;          /* maximum value of k */
    seq_t sum = 0;               /* accumulator for the sum */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t nmk = n - 3 * k;   /* compute n-3k */
        seq_t c   = binomial(nmk, k);           /* C(n-3k, k) */
        seq_t term = pow2(k) * c * c;          /* 2^k * C(n-3k,k)^2 */
        sum += term;                              /* add to the total */
    }
    return sum;
}
