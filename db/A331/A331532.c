/*
 * A331532  a(n) is the number of nonnegative integers k such that (n^2) AND
 *          (k^2) = k^2 (where AND denotes the bitwise AND operator).
 * https://oeis.org/A331532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k (0 ≤ k ≤ n) such that (k*k) & (n*n) == k*k
 * Range:     u8 n <= 22, u16 n <= 2983, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n2    = (seq_t)n * (seq_t)n;
    idx_t k     = 0;
    while (1) {
        if (k > n) break;               /* safety for huge n */
        seq_t k2 = (seq_t)k * (seq_t)k;
        if ((k2 & n2) == k2) count++;   /* k^2 is a submask of n^2 */
        if (k == n) break;
        ++k;
    }
    return count;
}
