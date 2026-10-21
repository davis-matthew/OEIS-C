/*
 * A281376  Total number of counts where floor(N/k) < floor((N+k)/n) for k =
 *          {1, 2, ..., n-1} and N >= n.
 * https://oeis.org/A281376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d=1..ceil((n-3)/3)} Sum_{j=1..n-(2*d+1)} floor(j/d)
 * Range:     u8 n <= 20, u16 n <= 195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 3) return 0;

    idx_t dmax = (n > 3) ? ((n - 3 + 2) / 3) : 0;
    seq_t total = 0;

    for (idx_t d = 1; d <= dmax; ++d) {
        idx_t m = n - (2 * d + 1);   /* 1 <= m for n >= 4 and d <= dmax   */
        if (m == 0) continue;        /* safety – m never negative as unsigned */

        idx_t q = m / d;
        idx_t r = m % d;
        if (q == 0) continue;        /* sum is zero if q==0                 */

        /* Sum_{j=1..m} floor(j/d) = d * q * (q-1) / 2 + q * (r+1)   */
        seq_t term = (seq_t)d * (seq_t)q * (seq_t)(q - 1) / 2
                   + (seq_t)q * (seq_t)(r + 1);

        total += term;
    }

    return total;
}
