/*
 * A390058  E.g.f. A(x) satisfies A(x) = exp( x * (1+x^2) * A(x)^2 ).
 * https://oeis.org/A390058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (2*(n-2k)+1)^(n-2k-1) *
 *            binomial(n-2k,k)/(n-2k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/big_pow.h"    /* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return 1;

    seq_t total = 0;
    idx_t max_k = n / 3;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t m = n - 2 * k;                     /* m = n-2k, ≥1 */

        /* factorial ratio n! / m! = product from m+1 to n */
        seq_t P = 1;
        for (idx_t i = m + 1; i <= n; ++i)
            P *= (seq_t)i;

        seq_t bin = binomial(m, k);

        seq_t base = (seq_t)(2 * m + 1);
        seq_t exp  = (seq_t)(m - 1);

        seq_t pow = big_pow(base, exp);

        seq_t term = bin * pow;
        term *= P;
        total += term;
    }

    return total;
}
