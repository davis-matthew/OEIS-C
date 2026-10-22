/*
 * A394974  a(n) = number of pairs (c,d) of integers with 1 <= c <= n-1 and 1
 *          <= d <= n-1 such that n*c + d divides (n-1)*d.
 * https://oeis.org/A394974
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{n-2} ⌊ (n-1) / ( n*k / (gcd(k+1,n) * gcd(k,n-1))
 *            ) ⌋
 * Range:     u8 n <= 4199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local gcd for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;            /* values below offset are irrelevant */

    idx_t n_minus_1 = n - 1;
    idx_t n_minus_2 = n - 2;
    seq_t total = 0;

    for (idx_t k = 1; k <= n_minus_2; ++k) {
        idx_t g1 = gcd_idx(k + 1, n);
        idx_t g2 = gcd_idx(k, n_minus_1);
        idx_t denom = g1 * g2;                    /* denom ≥ 1 */

        /* Compute Lk = n*k / denom, guaranteed to be integer */
        unsigned __int128 num = (unsigned __int128)n * k;
        idx_t Lk = (idx_t)(num / denom);

        /* (n-1)/Lk is the floor of the division */
        seq_t term = (seq_t)(n_minus_1 / Lk);
        total += term;
    }

    return total;
}
