/*
 * A323118  a(n) = U_{n}(n) where U_{n}(x) is a Chebyshev polynomial of the
 *          second kind.
 * https://oeis.org/A323118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n^2-1)^k * n^(n-2k) *
 *            binomial(n+1,2k+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t n_seq      = (seq_t)n;
    seq_t n_sq_minus = n_seq * n_seq - (seq_t)1;   /* n^2-1 */

    seq_t power_term = (seq_t)1;                   /* (n^2-1)^k      */
    seq_t power_n    = seq_pow(n_seq, n);          /* n^n            */
    seq_t sum        = (seq_t)0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        /* term = (n^2-1)^k * n^(n-2k) * binomial(n+1,2k+1) */
        seq_t term = power_term * power_n;
        term *= binomial(n + 1, 2 * k + 1);
        sum += term;

        /* update powers for next iteration */
        power_term *= n_sq_minus;          /* (n^2-1)^(k+1) */
        power_n    /= (n_seq * n_seq);     /* n^(n-2(k+1)) */
    }

    return sum;
}
