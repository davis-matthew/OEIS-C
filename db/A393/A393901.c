/*
 * A393901  Triangle read by rows: T(n,k) = (2*n + k - 1) * 2^(k-2).
 * https://oeis.org/A393901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k), where r is the row such that r(r+1)/2 >= n, k = n
 *            - r(r-1)/2, and T(r,k) = (2*r + k - 1) * 2^(k-2)
 * Range:     u8 n <= 20, u16 n <= 90, u32 n <= 405, u64 n <= 1769,
 *            u128 n <= 7502, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 3, ... */

#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* Undefined for n == 0, just return 0 */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cumulative = 1;        /* sum_{i=1}^{r} i */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Number of terms before row r */
    idx_t prev_cumulative = cumulative - r;
    idx_t k = n - prev_cumulative;   /* 1 <= k <= r */

    if (k == 1)
        return (seq_t)r;        /* T(r,1) = r */

    /* Compute (2*r + k - 1) * 2^(k-2) */
    seq_t base = (((seq_t)r) << 1) + (k - 1);   /* 2*r + k - 1 */
    return base << (k - 2);                     /* multiply by 2^(k-2) */
}
