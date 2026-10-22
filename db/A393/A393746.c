/*
 * A393746  Triangle read by rows: T(n, k) is the number of ordered pairs (i,
 *          j) of integers with 0 <= i, j < n such that i*j = k*n, for n>=1
 *          and k=0 to n-1.
 * https://oeis.org/A393746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8256, u16 n <= 9750, u32 n <= 9750, u64 n <= 9750,
 *            u128 n <= 9750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for a(n):
   Let r be the unique integer with r(r+1)/2 ≥ n > (r-1)r/2.
   Let k = n – (r−1)r/2 – 1  (0 ≤ k < r).
   Then a(n) = T(r,k) where
       T(r,0) = 2·r − 1
       T(r,k) = #{ d : k < d < r  and  (d divides k·r) }   (k > 0)
   This is the entry in row r and column k of the triangle A393746
   (rows are read left‑to‑right, top‑to‑bottom). */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t N)
{
    if (N == 0)           /* undefined for n<1, but return 0 */
        return (seq_t)0;

    /* Find r such that r*(r+1)/2 >= N > (r-1)*r/2. */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < N)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number (1‑based) */
    idx_t prev = (idx_t)((r - 1) * (idx_t)r / 2);   /* sum of first r‑1 rows */
    idx_t k = N - prev - 1;            /* column index (0‑based) */

    /* T(r,0) = 2r − 1 */
    if (k == 0)
        return (seq_t)(2 * r - 1);

    /* Count divisors d with k < d < r that divide k*r */
    idx_t prod = k * r;                /* fits in idx_t for n up to ~2^32 */
    seq_t cnt = 0;
    for (idx_t d = k + 1; d < r; ++d) {
        if (prod % d == 0)
            ++cnt;
    }
    return cnt;
}
