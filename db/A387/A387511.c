/*
 * A387511  a(n) = Sum_{k=0..floor(n/3)} 3^k * 2^(n-2*k) *
 *          binomial(n-2*k,k)^2.
 * https://oeis.org/A387511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 3^k * 2^(n-2*k) * binomial(n-2*k,
 *            k)^2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t pow3k = pow3(k);                 /* 3^k       */
        idx_t exp = n - 2 * k;                 /* exponent for 2 */
        seq_t pow2exp = pow2(exp);             /* 2^(n-2k)   */
        seq_t c = binomial(n - 2 * k, k);      /* C(n-2k, k) */
        seq_t term = pow3k;
        term *= pow2exp;
        term *= c;
        term *= c;
        sum += term;
    }
    return sum;
}
