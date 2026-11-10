/*
 * A266537  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          the twice odd numbers (A016825) interleaved with 2*k-1 zeros, and
 *          the first positive element of column k is in the row A002378(k),
 *          with T(1,1) = 0.
 * https://oeis.org/A266537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k)=4*((n-k(k+1))/(2k))+2 if n>=k(k+1) and 2k | (n-k(k+1)),
 *            else 0; row n has max(1,k: k(k+1)<=n) entries; a(n) is the
 *            triangle read by rows
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t sum = 0;
    idx_t r = 1;
    idx_t K = 1; /* row length of row r */
    for (;;) {
        while ((K + 1) * (K + 2) <= r) K++;
        if (sum + K >= n) break;
        sum += K;
        r++;
    }
    idx_t k = n - sum;
    idx_t start = k * (k + 1);
    if (r < start) return 0;
    idx_t d = r - start;
    if (d % (2 * k) != 0) return 0;
    return (seq_t)(d / (2 * k)) * 4 + 2;
}
