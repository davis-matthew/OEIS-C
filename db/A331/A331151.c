/*
 * A331151  Triangle read by rows: T(n,k) (n>=k>=1) =
 *          ceiling((n/k)*floor(n/k)).
 * https://oeis.org/A331151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ceil((m/k) * floor(m/k)), where (m,k) is the position of
 *            a(n) in the triangle read by rows: m = row number (smallest with
 *            m(m+1)/2 ≥ n) k = n - (m-1)m/2 (column index, 1 ≤ k ≤ m)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term; return 0 for n==0 only to avoid UB */
    if (n == 0) return (seq_t)0;

    /* Find the row m such that m(m+1)/2 ≥ n (smallest m). */
    unsigned __int128 nn = n;              /* uint64_t fits in 128 bits */
    unsigned __int128 low = 1, high = 1;
    /* Expand high until it is large enough. No overflow because m < 2^32 for any
       64‑bit n. */
    while (high * (high + 1) / 2 < nn) { high <<= 1; }

    /* Binary search for minimal m */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= nn)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = (idx_t)low;            /* row number */

    /* Column index k = n - (m-1)m/2 */
    unsigned __int128 prevSum = ((unsigned __int128)(m - 1)) * m / 2;
    idx_t k = (idx_t)(nn - prevSum);

    /* f = floor(m/k) */
    idx_t f = m / k;

    /* a(n) = ceil( (m * f) / k ) = (m*f + k - 1)/k  */
    seq_t result = (seq_t)(( (unsigned __int128)m * f + k - 1 ) / k);
    return result;
}
