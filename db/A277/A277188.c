/*
 * A277188  The binomial sum a(n) =
 *          Sum_{k=0..n}(binomial(n,k)*binomial(n+1,k+1)*binomial(n+2,k+2)).
 * https://oeis.org/A277188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+1,k+1) *
 *            binomial(n+2,k+2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a multiplicative recurrence for the binomial
 * coefficients, avoiding factorials and keeping the intermediate values as
 * small as possible. This works for any seq_t type (8/16/32/64/128 bit or
 * arbitrary precision) and requires only the arithmetic operators defined
 * for seq_t. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t cnk   = 1;                 /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin1 = cnk * (seq_t)(n + 1) / (seq_t)(k + 1);   /* C(n+1,k+1) */
        seq_t bin2 = bin1 * (seq_t)(n + 2) / (seq_t)(k + 2);   /* C(n+2,k+2) */
        sum += cnk * bin1 * bin2;
        if (k < n) {                                 /* update C(n,k+1) */
            cnk = cnk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
