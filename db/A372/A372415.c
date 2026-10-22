/*
 * A372415  Coefficient of x^n in the expansion of ( (1-x+x^3) / (1-x)^3 )^n.
 * https://oeis.org/A372415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(3*n-2*k-1,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 separately to avoid negative arguments to binomial. */
    if (n == 0u)
        return (seq_t)1;

    seq_t total = (seq_t)0;
    idx_t max_k = n / 3u;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t c1 = binomial(n, k);

        /* The second binomial argument always satisfies 0 ≤ n-3*k ≤ 3*n-2*k-1 for n>0. */
        idx_t t   = 3 * n - 2 * k - 1u;
        idx_t k2  = n - 3 * k;
        seq_t c2  = binomial(t, k2);

        total += c1 * c2;
    }

    return total;
}
