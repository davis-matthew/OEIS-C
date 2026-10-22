/*
 * A363983  a(n) = Sum_{k = floor((n+1)/2)..n}
 *          (-1)^(n+k)*binomial(n,k)*binomial(n+k-1,k)*binomial(2*k,n).
 * https://oeis.org/A363983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = floor((n+1)/2)..n} (-1)^(n+k) *
 *            binomial(n,k)*binomial(n+k-1,k)*binomial(2*k,n) (OEIS A363983)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    idx_t start = (n + 1) / 2;           /* floor((n+1)/2) */
    seq_t pos = 0;                       /* sum of terms with (+) sign */
    seq_t neg = 0;                       /* sum of terms with (-) sign */

    for (idx_t k = start; k <= n; ++k) {
        /* term = C(n,k) * C(n+k-1,k) * C(2*k,n) */
        seq_t term = binomial(n, k);
        term = seq_mul(term, binomial(n + k - 1, k));
        term = seq_mul(term, binomial(2 * k, n));

        if (((n + k) & 1) == 0)   /* (-1)^(n+k) is +1 */
            pos += term;
        else                      /* (-1)^(n+k) is -1 */
            neg += term;
    }

    /* final value: pos - neg (always non‑negative) */
    return pos - neg;
}
