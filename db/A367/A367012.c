/*
 * A367012  a(n) = Sum_{k=0..n} k! * (n-k)^k.
 * https://oeis.org/A367012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * (n-k)^k
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

/* Computes a(n) = Sum_{k=0..n} k! * (n-k)^k.
 *  idx_t is the index type, seq_t is the sequence value type.
 *
 *  The implementation uses an incremental factorial to avoid
 *  storing all factorial values, and the helper big_pow to
 *  compute (n-k)^k efficiently.
 */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t fact   = (seq_t)1;             /* k!   (starts with 0! = 1) */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        seq_t pow = (seq_t)1;            /* default for k == 0 */

        if (k != (idx_t)0) {
            pow = big_pow((seq_t)(n - k), (seq_t)k);
        }

        seq_t term = fact * pow;         /* k! * (n-k)^k   */
        result += term;

        fact *= (seq_t)(k + (idx_t)1);   /* prepare (k+1)! for next step */
    }

    return result;
}
