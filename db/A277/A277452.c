/*
 * A277452  a(n) = Sum_{k=0..n} binomial(n,k) * n^k * k!.
 * https://oeis.org/A277452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * n^k * k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the recurrence:
   term_0 = 1
   term_{k+1} = term_k * n * (n - k)
   a(n)    = sum_{k=0..n} term_k
*/

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t term   = 1;

    for (idx_t k = 0; k <= n; ++k) {
        result += term;
        if (k < n) {
            term = term * (seq_t)n * (seq_t)(n - k);
        }
    }

    return result;
}
