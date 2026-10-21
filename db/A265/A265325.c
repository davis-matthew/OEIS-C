/*
 * A265325  Number of n-digit decimal numbers with leading zeros allowed where
 *          no digit appears exactly twice.
 * https://oeis.org/A265325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k=0}^{min(10,⌊n/2⌋)} (-1)^k * C(10,k) *
 *            n!/(2^k*(n-2k)!) * (10-k)^{\,n-2k}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS sequence A265325 */

#include <stdint.h>
#include <stdbool.h>   /* for bool, if needed */
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"  /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = 10;
    if (n/2 < maxk) maxk = n/2;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial coefficient C(10,k) */
        seq_t coeff = binomial(10, k);

        /* product n!/(n-2k)!  ==  Π_{i=n-2k+1}^{n} i */
        seq_t prod = 1;
        for (idx_t i = n - 2*k + 1; i <= n; ++i) {
            prod *= (seq_t)i;
        }

        /* divide by 2^k */
        prod /= ((seq_t)1 << k);

        /* multiply by (10-k)^(n-2k) */
        seq_t powv = seq_pow((seq_t)(10 - k), n - 2*k);
        prod *= powv;

        /* multiply by the coefficient */
        prod *= coeff;

        /* add or subtract from result depending on parity of k */
        if (k & 1) {
            result -= prod;
        } else {
            result += prod;
        }
    }

    return result;
}
