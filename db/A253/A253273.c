/*
 * A253273  Triangle T(n,k) = Sum_{j=0..n-k+1}
 *          binomial(k+j,k-j+1)*binomial(n-k,j-1), read by rows.
 * https://oeis.org/A253273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where m = ⌊(−1+√(8n+1))/2⌋, k = n − m(m+1)/2, and
 *            T(m,k) = Σ_{j=1}^{m−k+1} binomial(k+j, k−j+1) * binomial(m−k,
 *            j−1)
 * Range:     u8 n <= 40, u16 n <= 127, u32 n <= 449, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find m such that m(m+1)/2 <= n < (m+1)(m+2)/2 */
    idx_t m = 0;
    idx_t t = 0;          /* current triangular number t = m(m+1)/2 */
    while (t + m + 1 <= n) {
        t += m + 1;
        ++m;
    }
    idx_t k = n - t;    /* position within the row */

    seq_t result = 0;
    idx_t max_j = m - k + 1;
    for (idx_t j = 1; j <= max_j; ++j) {
        seq_t term1 = binomial(k + j, k - j + 1);
        term1 *= binomial(m - k, j - 1);
        result += term1;
    }
    return result;
}
