/*
 * A386940  a(n) = Sum_{k=0..n} binomial(2*k,k) * binomial(2*n-k-1,n-k).
 * https://oeis.org/A386940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k) * binomial(2*n-k-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    /* central_k = binomial(2*k,k) initialized for k=0 */
    seq_t central = (seq_t)1;

    /* m = binomial(2*n-1, n) -> corresponds to k=0 */
    seq_t m = binomial((idx_t)(2 * n - 1), (idx_t)n);

    /* iterate k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* add term for current k */
        sum += central * m;

        /* stop after last term */
        if (k == n) break;

        /* update central to binomial(2*(k+1),k+1)
           central = central * (4*k+2)/(k+1)   (exact integer division) */
        central = (central * (seq_t)(4 * k + 2)) / (seq_t)(k + 1);

        /* update m to binomial(2*n-(k+1)-1, n-(k+1))
           m = m * (n-k)/(2*n-k-1)  (exact integer division) */
        m = (m * (seq_t)(n - k)) / (seq_t)(2 * n - k - 1);
    }

    return sum;
}
