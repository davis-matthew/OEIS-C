/*
 * A271647  Irregular triangle read by rows: the natural numbers from right to
 *          left.
 * https://oeis.org/A271647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A271647 – Irregular triangle read by rows (right to left)
 *
 *  The natural numbers are partitioned into rows of lengths
 *  1,1,2,2,3,3,4,4,…; each row is written in reverse order.
 *  The rows are concatenated to produce the sequence
 *  a(1)=1, a(2)=2, a(3)=4, a(4)=3, a(5)=6, a(6)=5, …
 *
 *  Algorithm:
 *    1. Compute m = ⌊√n⌋  (integer square root).
 *    2. Determine which row the index n falls into:
 *         if n ≤ m²          → odd row r = 2m−1, length L = m
 *         else if n ≤ m(m+1) → even row r = 2m,   length L = m
 *         else               → odd row r = 2(m+1)−1, length L = m+1
 *    3. Let S_prev be the number of terms before this row,
 *       S_row  be the total number of terms up to this row.
 *       For odd rows:
 *            S_prev = (L−1)·L,   S_row = L·L
 *       For even rows:
 *            S_prev = L·L,       S_row = L·(L+1)
 *    4. pos = n − S_prev (1‑based position in the row)
 *       a(n) = S_row − (pos − 1)
 *
 *  All arithmetic is performed with unsigned integer types.
 *  The helper integer square root is implemented locally.
 */

#include <stdint.h>   /* for __uint128_t */

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t mid2 = ( __uint128_t )mid * mid;
        if (mid2 == n) return mid;
        if (mid2 < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 0 */

    idx_t m = floor_sqrt_u64(n);

    if (n <= m * m) {                /* odd row r = 2m−1, length L = m */
        idx_t L = m;
        seq_t S_prev = (seq_t)(L - 1) * (seq_t)L;
        seq_t S_row  = (seq_t)L * (seq_t)L;
        seq_t pos    = (seq_t)n - S_prev;
        return S_row - pos + 1;
    } else if (n <= m * (m + 1)) {    /* even row r = 2m,   length L = m */
        idx_t L = m;
        seq_t S_prev = (seq_t)L * (seq_t)L;
        seq_t S_row  = (seq_t)L * (seq_t)(L + 1);
        seq_t pos    = (seq_t)n - S_prev;
        return S_row - pos + 1;
    } else {                          /* odd row r = 2(m+1)−1, length L = m+1 */
        idx_t L = m + 1;
        seq_t S_prev = (seq_t)(L - 1) * (seq_t)L;
        seq_t S_row  = (seq_t)L * (seq_t)L;
        seq_t pos    = (seq_t)n - S_prev;
        return S_row - pos + 1;
    }
}
