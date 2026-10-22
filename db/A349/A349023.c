/*
 * A349023  G.f. satisfies: A(x) = 1/(1 - x/(1 - x*A(x))^4)^2.
 * https://oeis.org/A349023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(2*n - k + 1, k) * binomial(n + 3*k -
 *            1, n-k) / (n-k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* First binomial term: binomial(2*n - k + 1, k) */
        idx_t first_n = 2 * n - k + 1;
        seq_t first = binomial(first_n, k);

        /* Second binomial term */
        seq_t second;
        if (n == 0 && k == 0)
            second = 1;
        else
            second = binomial(n + 3 * k - 1, n - k);

        /* Multiply and divide by the denominator (n-k+1) */
        seq_t prod = first * second;
        seq_t denom = (seq_t)(n - k + 1);
        total += prod / denom;
    }
    return total;
}
