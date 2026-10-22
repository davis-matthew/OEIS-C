/*
 * A362337  a(n) = n! * Sum_{k=0..floor(n/2)} (-k)^k / (k! * (n-2*k)!).
 * https://oeis.org/A362337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (-k)^k / (k! * (n-2*k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* we compute the sum by iterating over k and updating the combinatorial
       factor C(k) = n!/(k! * (n-2k)!) incrementally.  The factor is always
       integer, so we avoid expensive factorial divisions. */
    seq_t sum = 0;
    seq_t C   = 1;                     /* C(0) = 1 */
    idx_t m   = n / 2;                 /* floor(n/2) */

    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t term;
        if (k == 0)
            term = C;                 /* (-0)^0 = 1 */
        else
        {
            seq_t pow_k = pow_seq( (seq_t)(- (long long)k), (int)k );
            term = C * pow_k;
        }
        sum += term;

        if (k < m)  /* update C to C(k+1) */
        {
            seq_t mul1 = (seq_t)(n - 2*k);
            seq_t mul2 = (seq_t)(n - 2*k - 1);
            seq_t num  = mul1 * mul2;                  /* (n-2k)*(n-2k-1) */
            seq_t denom = (seq_t)(k + 1);             /* small divisor */
            C = C * num / denom;
        }
    }

    return sum;
}
