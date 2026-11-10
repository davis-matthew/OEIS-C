/*
 * A380899  Three-Catalan Triangle read by rows, for n>=0 and k>=0.
 * https://oeis.org/A380899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n, k) = A008287(2*n, 3*n+k) - A008287(2*n, 3*n+k+1), where
 *            A008287(r, j) = Sum_{i} binomial(r, i)*binomial(r, j-2*i) is the
 *            coefficient of x^j in (1+x+x^2+x^3)^r; row n has 3*n+1 entries,
 *            k = 0..3*n
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-10)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* coefficient of x^j in (1+x)^r (1+x^2)^r = (1+x+x^2+x^3)^r */
static seq_t A380899_coef(idx_t r, idx_t j)
{
    seq_t result = 0;
    if (j > 3 * r) return result;
    for (idx_t i = 0; 2 * i <= j; ++i)
    {
        if (i > r) break;
        idx_t m = j - 2 * i;
        if (m > r) continue;
        result += binomial(r, i) * binomial(r, m);
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t row = 0;
    for (;; ++row)
    {
        idx_t next = (row + 1) * (3 * (row + 1) - 1) / 2;
        if (n < next) break;
    }
    idx_t start = row * (3 * row - 1) / 2;
    idx_t k = n - start;
    idx_t r = 2 * row;
    idx_t j = 3 * row + k;
    seq_t hi = A380899_coef(r, j);
    seq_t lo = A380899_coef(r, j + 1);
    return hi - lo;
}
