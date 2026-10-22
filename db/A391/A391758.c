/*
 * A391758  a(n) = Sum_{k=0..floor(n/4)} (n-3*k+1) * 2^k * 3^(n-3*k) *
 *          binomial(2*(n-3*k),2*k).
 * https://oeis.org/A391758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} (n-3k+1) * 2^k * 3^{n-3k} * * *
 *            binomial(2*(n-3k), 2k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
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
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 3 * k;                /* m = n-3k  */
        seq_t term = binomial(2 * m, 2 * k);
        term *= pow2(k);                    /* multiply by 2^k */
        term *= pow3(m);                    /* multiply by 3^{n-3k} */
        term *= (seq_t)(m) + 1;             /* multiply by (m+1) = (n-3k+1) */
        sum += term;
    }
    return sum;
}
