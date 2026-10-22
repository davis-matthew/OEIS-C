/*
 * A331969  T(n, k) = [x^(n-k)] 1/(((1 - 2*x)^k)*(1 - x)^(k + 1)). Triangle
 *          read by rows, for 0 <= k <= n.
 * https://oeis.org/A331969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, triangle_terms_until
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the flattened value of the triangle T(r,k)
 * defined by 
 *        T(r,k) = [x^(r-k)] 1 / ((1-2x)^k * (1-x)^(k+1))
 * for 0 ≤ k ≤ r. 
 * The function maps a flattened index n to the corresponding
 * pair (r,k) and evaluates the coefficient using the binomial
 * sum
 *        T(r,k) = Σ_{p=0}^{r-k} binom(k+p-1,p) * 2^p * binom(k+r-k-p, r-k-p)
 */

#include "../utils/binom.h"
#include "../utils/triangle_terms_until.h"

seq_t a(idx_t n)
{
    /* Find the row r such that triangle_terms_until(r-1) <= n < triangle_terms_until(r) */
    idx_t low = 0, high = 1;
    while (triangle_terms_until(high) <= n) {
        high <<= 1;                    /* double until we exceed n */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangle_terms_until(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row index of n */
    idx_t prev_terms = (r == 0) ? 0
                               : triangle_terms_until(r - 1);
    idx_t k = n - prev_terms;          /* column within the row */

    if (k == 0) return (seq_t)1;       /* T(r,0) = 1 for all r */

    idx_t m = r - k;                    /* exponent for x in the generating function */

    seq_t sum = 0;
    seq_t pow2 = 1;                     /* 2^p */
    seq_t c1   = 1;                     /* binom(k+p-1,p) for current p */
    seq_t c2   = binom(k + m, m);       /* binom(k+q,q) for current q = m-p */

    for (idx_t p = 0; p <= m; ++p) {
        sum += c1 * pow2 * c2;

        if (p == m) break;             /* last term processed */

        /* Update for next p (p → p+1) */
        c1 = c1 * (seq_t)(k + p) / (seq_t)(p + 1);
        pow2 = pow2 * (seq_t)2;

        /* q decreases by 1: from q = m-p to q = m-p-1 */
        idx_t q_old = m - p;
        c2 = c2 * (seq_t)q_old / (seq_t)(k + q_old);
    }

    return sum;
}
