/*
 * A272025  Irregular triangle read by rows, n >= 1, 1 <= k <= A038548(n), in
 *          which T(n,k) is the sum of the k-th pair of conjugate divisors of
 *          n, or T(n,k) is the central divisor of n if such a pair does not
 *          exist.
 * https://oeis.org/A272025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272025
 *  Irregular triangle read by rows:  T(n,k) = d + n/d if d is a divisor of n
 *  (or d if d = n/d).  The sequence a(n) is the triangle flattened
 *  row‑by‑row, starting with a(1)=1.
 *  The function returns a(n) for n ≥ 1.  a(0) is undefined and returns 0.
 */

#include <stdint.h>

static inline idx_t int_sqrt_uint64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* Avoid multiplication overflow: compare mid with n / mid */
        if (mid > n / mid) high = mid - 1;
        else low = mid + 1;
    }
    return high;
}

/* Number of divisor‑pair entries in row n
   (i.e. the count of divisors d ≤ √n). */
static inline idx_t row_len(idx_t n)
{
    idx_t len = 0;
    idx_t lim = int_sqrt_uint64(n);
    for (idx_t d = 1; d <= lim; ++d) {
        if (n % d == 0) ++len;
    }
    return len;
}

/* Return a(n) for the irregular triangle (OEIS A272025). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 */
    idx_t remaining = n;           /* how many terms still to skip */
    idx_t m = 1;                   /* current row */

    for (;;) {
        idx_t len = row_len(m);
        if (remaining > len) {
            remaining -= len;      /* skip this whole row */
            ++m;
            continue;
        }
        /* remaining ≤ len: the answer is in row m at position remaining */
        idx_t lim = int_sqrt_uint64(m);
        idx_t cnt = 0;
        for (idx_t d = 1; d <= lim; ++d) {
            if (m % d == 0) {
                ++cnt;
                if (cnt == remaining) {
                    idx_t q = m / d;
                    if (d == q) {            /* perfect square case */
                        return (seq_t)d;
                    } else {
                        return (seq_t)d + (seq_t)q;
                    }
                }
            }
        }
        /* should never reach here */
        return 0;
    }
}
