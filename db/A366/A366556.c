/*
 * A366556  G.f. A(x) satisfies A(x) = 1 + x + x^4*A(x)^3.
 * https://oeis.org/A366556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(2*k+1, n-4*k) * *
 *            binomial(3*k, k) / (2*k+1)
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */
#include "../utils/gcd.h"        /* provides seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t t = n - 4 * k;                     /* n-4k */
        seq_t bin1 = binomial(2 * k + 1, t);     /* C(2k+1, n-4k) */
        if (bin1 == 0) continue;                /* term vanishes */
        seq_t bin2 = binomial(3 * k, k);         /* C(3k, k) */
        seq_t denom = (seq_t)(2 * k + 1);        /* (2k+1) */

        /* Reduce bin1 and bin2 by common factors with denom */
        seq_t g = gcd(bin1, denom);
        bin1 /= g; denom /= g;
        g = gcd(bin2, denom);
        bin2 /= g; denom /= g;

        /* denom should now be 1, but keep the division for safety */
        seq_t term = (bin1 * bin2) / denom;
        sum += term;
    }
    return sum;
}
