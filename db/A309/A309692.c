/*
 * A309692  Sum of the odd parts appearing among the largest parts of the
 *          partitions of n into 3 parts.
 * https://oeis.org/A309692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=1..⌊n/3⌋} Σ_{i=j..⌊(n-j)/2⌋} (n‐i‐j) * ((n‐i‐j) mod
 *            2)
 * Range:     u8 n <= 22, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of odd largest parts among all partitions of n into 3 parts. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n < 3) return res;                /* no partitions of 3 parts for n < 3 */

    idx_t jmax = n / 3;                    /* maximum value of the smallest part j */

    for (idx_t j = 1; j <= jmax; ++j) {
        idx_t imax = (n - j) / 2;          /* j ≤ i ≤ (n-j)/2 ensures a≤b≤c */
        for (idx_t i = j; i <= imax; ++i) {
            idx_t c = n - i - j;          /* largest part of the partition */
            if (c & 1) {                  /* add only if c is odd */
                res += (seq_t)c;
            }
        }
    }
    return res;
}
