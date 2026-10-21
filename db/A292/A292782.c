/*
 * A292782  a(n) = E(2n,n)/2, where E(n,x) is the Euler polynomial.
 * https://oeis.org/A292782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * sum_{k=1}^{n-1} (-1)^{k+1} k^{2n}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        = sum_{k=1}^{n-1} (-1)^{n+k+1} k^{2n} */
/*        = pos - neg where pos contains terms with (n+k) odd, neg with (n+k) even. */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    seq_t pos = 0;
    seq_t neg = 0;
    idx_t exp = 2 * n;               /* exponent 2n, fits in idx_t */

    for (idx_t k = 1; k < n; ++k) {
        seq_t term = seq_pow((seq_t)k, exp);
        /* (-1)^(n+k+1) is +1 when (n+k) is odd, -1 when even */
        if ((n + k) & 1u) {
            pos += term;
        } else {
            neg += term;
        }
    }

    return pos - neg;                 /* final a(n) */
}
