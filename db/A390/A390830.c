/*
 * A390830  a(n) = Sum_{k=0..floor(n/4)} 2^k * 3^(n-3*k) * binomial(n-2*k,k) *
 *          binomial(n-3*k,k).
 * https://oeis.org/A390830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 2^k * 3^(n-3k) * binomial(n-2k,k) *
 *            binomial(n-3k,k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
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
    seq_t total = 0;
    idx_t maxk = n / 4; /* floor(n/4) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term = pow2(k);
        term *= pow3(n - 3 * k);
        term *= binomial(n - 2 * k, k);
        term *= binomial(n - 3 * k, k);
        total += term;
    }
    return total;
}
