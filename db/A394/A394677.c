/*
 * A394677  Row sums of A394670.
 * https://oeis.org/A394677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k) with T(n,k) =
 *            n!*Sum_{m=1..floor(n/k)} binomial(n-(k-1)*m-1, m-1) for k>=1,
 *            T(n,0)=0 for n>0, a(0)=1 (T(n,k) = n!*[x^n] 1/(1-x^k/(1-x)),
 *            A394670)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-10)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m_max = n / k;
        for (idx_t m = 1; m <= m_max; ++m) {
            idx_t top = n - (k - 1) * m - 1;
            sum += binomial(top, m - 1);
        }
    }

    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    return fact * sum;
}
