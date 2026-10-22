/*
 * A344435  a(n) = Sum_{k=1..n} (A000292(n) mod A000217(k)).
 * https://oeis.org/A344435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ( C(n+2,3) mod C(k+1,2) )
 * Range:     u8 n <= 10, u16 n <= 91, u32 n <= 2262, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = Sum_{k=1..n} ( ((n+2)(n+1)n/6) mod ((k+1)k/2) ) */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    idx_t i;
    seq_t N = (seq_t)n;
    /* Compute binomial(n+2,3) = n(n+1)(n+2)/6  */
    seq_t W = (N * (N + 1) * (N + 2)) / 6;

    seq_t sum = 0;
    for (i = 1; i <= n; ++i) {
        seq_t K = (seq_t)i;
        seq_t T = (K * (K + 1)) / 2;         /* binomial(k+1,2) */
        sum += W % T;
    }
    return sum;
}
