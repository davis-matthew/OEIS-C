/*
 * A374676  a(n) = Sum_{k=0..n} binomial(n+k-1,n) * binomial(n+k,n)^2.
 * https://oeis.org/A374676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,n) * binomial(n+k,n)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) = 1 */

    seq_t sum = 0;
    seq_t B = 1;  /* B(k) = C(n+k, n), initially B(0) = C(n, n) = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial B(k) = C(n+k, n) from B(k-1) */
        B = B * ((seq_t)n + k) / (seq_t)k;

        /* A(k) = C(n+k-1, n) = B(k) * k / (n+k) */
        seq_t A = B * (seq_t)k / ((seq_t)n + k);

        /* term = A * B^2 */
        seq_t term = A * B * B;

        sum += term;
    }

    return sum;
}
