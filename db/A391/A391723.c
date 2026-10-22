/*
 * A391723  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) * binomial(k+6,6) *
 *          binomial(2*k,2*(n-3*k)).
 * https://oeis.org/A391723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) * binomial(k+6,6) *
 *            binomial(2*k,2*(n-3*k))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 23, u64 n <= 47, u128 n <= 101,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                 /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t diff = n - 3 * k;         /* n - 3*k */

        seq_t term = pow2(k);            /* 2^k */
        term *= pow3(diff);              /* * 3^(n-3k) */
        term *= binomial(k + 6, 6);      /* * binomial(k+6,6) */
        term *= binomial(2 * k, 2 * diff); /* * binomial(2k,2*(n-3k)) */

        sum += term;
    }
    return sum;
}
