/*
 * A255483  Infinite square array read by antidiagonals downwards: T(0,m) =
 *          prime(m), m >= 1; for n >= 1, T(n,m) =
 *          T(n-1,m)*T(n-1,m+1)/gcd(T(n-1,m), T(n-1,m+1))^2, m >= 1.
 * https://oeis.org/A255483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the infinite array T(n,m) read * by
 *            antidiagonals: T(0,m)=prime(m), m≥1; * T(n,m)=∏_{k=0..n}
 *            prime(k+m)^{[C(n,k) odd]} * where C(n,k) is the binomial
 *            coefficient. * The antidiagonal order is: * for d=1,2,3,…, *
 *            i=0..d-1, j=d-i, (row i, column j) * output T(i,j) in this o...
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 61, u64 n <= 134,
 *            u128 n <= 526, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* find diagonal d such that n < d*(d+1)/2 */
    idx_t d = 1;
    while ((d * (d + 1) / 2) <= n) ++d;

    /* number of entries before this diagonal */
    idx_t prev = (d - 1) * d / 2;
    idx_t offset = n - prev;           /* row index i */
    idx_t i = offset;
    idx_t j = d - i;                    /* column index m (≥1) */

    seq_t result = 1;
    for (idx_t k = 0; k <= i; ++k)
    {
        /* C(i,k) is odd iff (k & (i-k)) == 0 */
        if (((k & (i - k)) == 0))
        {
            /* multiply by prime(j+k) */
            result *= (seq_t)get_prime(j + k);
        }
    }
    return result;
}
