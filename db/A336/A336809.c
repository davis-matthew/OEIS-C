/*
 * A336809  a(n) = (n!)^2 * Sum_{k=0..n} (k+1) / ((n-k)!)^2.
 * https://oeis.org/A336809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{k=0..n} (k+1)/((n-k)!)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A336809.
 * The implementation uses the identity:
 *   a(n) = sum_{k=0..n} (k+1) * [ n*(n-1)*...*(n-k+1) ]^2
 * which can be evaluated in O(n) time with only integer arithmetic. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t P = 1;          /* Product n*(n-1)*...*(n-k+1) for current k */
    seq_t N = (seq_t)n;   /* Cast index to seq_t */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kplus1 = (seq_t)k + 1;
        seq_t term = kplus1 * P * P;   /* (k+1) * P^2 */
        result += term;
        if (k < n)
            P *= (N - (seq_t)k);      /* advance P to next k */
    }
    return result;
}
