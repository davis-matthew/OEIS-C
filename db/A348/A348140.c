/*
 * A348140  a(n) is the numerator of tan(n * arctan(1/n)).
 * https://oeis.org/A348140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = numerator of tan( n * arctan(1/n) )
    Recurrence (with fraction reduction after each step):

       P_1 = 1          , Q_1 = n
       P_{k+1} = P_k * n + Q_k
       Q_{k+1} = Q_k * n - P_k
       (P_k , Q_k) are kept coprime after every step

    The returned value is P_n after the sequence of reductions.
    This formula works for any seq_t that supports the usual
    integer arithmetic and a gcd routine. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 1 */
        return (seq_t)1;

    seq_t P = 1;              /* P_1 */
    seq_t Q = (seq_t)n;       /* Q_1 */
    seq_t nseq = (seq_t)n;

    for (idx_t k = 1; k < n; ++k) {
        /* new fraction before reduction */
        seq_t newP = P * nseq + Q;
        seq_t newQ = Q * nseq - P;

        /* reduce */
        seq_t g = gcd(newP, newQ);
        P = newP / g;
        Q = newQ / g;
    }
    return P;
}
