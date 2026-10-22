/*
 * A361887  a(n) = S(5,n), where S(r,n) = Sum_{k = 0..floor(n/2)} (
 *          binomial(n,k) - binomial(n,k-1) )^r.
 * https://oeis.org/A361887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} ( (n-2k+1)/(n-k+1) * binomial(n,k)
 *            )^5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    seq_t sum = 0;
    seq_t binom = 1;   /* binomial(n,0) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* term = binom * (n-2k+1) / (n-k+1) */
        seq_t num = binom * (seq_t)(n - 2*k + 1);
        seq_t den = (seq_t)(n - k + 1);
        seq_t term = num / den;   /* exact division */

        sum += big_pow(term, 5);   /* term^5 */

        /* update binom for next k, if not the last iteration */
        if (k < maxk) {
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    return sum;
}
