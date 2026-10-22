/*
 * A335309  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+k,k) * n^(n-k).
 * https://oeis.org/A335309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * (n+1)^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) for the OEIS sequence A335309
 *
 *   a(n) = \sum_{k=0}^{n} C(n,k)^2 * (n+1)^k
 *
 * The implementation uses O(n) arithmetic operations per call.
 * All arithmetic is performed on the seq_t type, which supports
 * +, -, *, /, +, -, <<, >> and bitwise operators.
 */
seq_t a(idx_t n)
{
    seq_t result = 0;          /* accumulator for the sum   */
    seq_t comb   = 1;          /* C(n,0) = 1                */
    seq_t pow    = 1;          /* (n+1)^0 = 1               */
    seq_t np1    = (seq_t)(n + 1);  /* n+1 as seq_t            */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = comb^2 * pow */
        seq_t term = (comb * comb) * pow;
        result += term;

        /* break when last term processed */
        if (k == n)
            break;

        /* update binomial coefficient for next k:
           C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        comb = (comb * (seq_t)(n - k)) / (seq_t)(k + 1);

        /* update power for next k:
           (n+1)^(k+1) = (n+1)^k * (n+1) */
        pow = pow * np1;
    }

    return result;
}
