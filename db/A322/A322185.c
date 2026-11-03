/*
 * A322185  a(n) = sigma(2*n) * binomial(2*n,n)/2, for n >= 1.
 * https://oeis.org/A322185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n) * binomial(2*n,n) / 2 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 471 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, return 0 */

    idx_t two_n = n * 2ULL;           /* 2*n */
    seq_t sigma = sigma_of(two_n);    /* σ(2*n) */

    /* central binomial coefficient: C(2n,n) */
    seq_t binom = binomial(two_n, n);

    /*
     * Either σ(2n) or C(2n,n) is even.
     * σ(2n) is odd only when n is a perfect square.
     * C(2n,n) is always even for n ≥ 1.
     * We divide the even factor by 2 to keep numbers small.
     */
    if ((sigma & 1ULL) == 0)          /* σ(2n) even */
        sigma >>= 1;                  /* σ(2n)/2 */
    else
        binom >>= 1;                  /* C(2n,n)/2 */

    return sigma * binom;              /* exact product */
}
