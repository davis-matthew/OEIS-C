/*
 * A306405  T(n,k) = k*Sum_{i=0..(n-k)/2} C(k,2*k+2*i-n)*C(k+2*i-1,i)/(k+i),
 *          triangle read by rows for n >= 1 and 1 <= k <= n.
 * https://oeis.org/A306405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) = k * Σ_{i=0}^{⌊(n-k)/2⌋} binom(k,2k+2i-n) * binom(k+2i-1,i) / (k+i);
   The sequence a(n) is obtained by reading T(n,k) row-wise:
   a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), ...
   First few terms: 1,1,1,1,2,1,1,3,3,1,2,4,6,4,1,2,7,10,10,5,1,5,10,18,...
*/

#include <stdint.h>
#include "../utils/binom.h"

extern seq_t binom(idx_t a, idx_t b);

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset is 1; behaviour for n<1 is undefined */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Number of elements before this row */
    idx_t prev_rows = (r - 1) * r / 2;

    /* Position within the row (1‑based) */
    idx_t k = n - prev_rows;   /* 1 <= k <= r */

    seq_t sum = 0;

    /* i ranges from 0 to ⌊(r-k)/2⌋ */
    idx_t max_i = (r > k) ? (r - k) / 2 : 0;
    for (idx_t i = 0; i <= max_i; ++i) {
        /* compute indices for binomials */
        long long m1 = 2LL * (long long)k + 2LL * (long long)i - (long long)r;
        if (m1 < 0 || (idx_t)m1 > k) continue;

        long long m2 = (long long)k + 2LL * (long long)i - 1;
        /* m2 is always >= 0 when k >= 1 and i >= 0 */

        seq_t c1 = binom(k, (idx_t)m1);
        if (c1 == 0) continue;

        seq_t c2 = binom(k + 2 * i - 1, i);
        if (c2 == 0) continue;

        seq_t term = c1 * c2;           /* C(k,m1) * C(k+2i-1,i) */
        term *= k;                      /* multiply by k */
        term /= (k + i);                /* divide by (k+i) */
        sum += term;
    }

    return sum;
}
