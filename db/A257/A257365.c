/*
 * A257365  Triangle, read by rows, T(n,k) = Sum_{m=0..(n-k)/2}
 *          C(k,m)*C(n-2*m,k).
 * https://oeis.org/A257365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{m=0}^{(r-k)/2} binomial(k,m)*binomial(r-2m,k), where
 *            r = floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 158, u32 n <= 508, u64 n <= 1853,
 *            u128 n <= 6835, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

static seq_t triangle_val(idx_t r, idx_t k)
{
    if (k > r) return 0;
    idx_t m_max = (r - k) / 2;
    seq_t sum = 0;
    for (idx_t m = 0; m <= m_max; ++m) {
        seq_t t1 = binomial(k, m);
        seq_t t2 = binomial(r - 2 * m, k);
        sum += t1 * t2;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    /* linear search is inexpensive for the expected range of n (≤ 100 000) */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;
    return triangle_val(r, k);
}
