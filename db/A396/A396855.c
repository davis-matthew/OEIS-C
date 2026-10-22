/*
 * A396855  Number of partitions of n into two parts with disjoint decimal
 *          digit sets.
 * https://oeis.org/A396855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 958, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A396855: number of partitions of n into two parts with disjoint decimal digit sets.
   For each unordered pair {a,b} with a+b=n and 1≤a≤b, count the pair if the decimal digit sets of a and b are disjoint. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over all unordered pairs (a,b) with a+b=n, a≤b */
    for (idx_t a_val = 1; a_val <= n / 2; ++a_val) {
        idx_t b_val = n - a_val;

        /* compute bitmask of digits present in a_val */
        unsigned int maskA = 0;
        idx_t temp = a_val;
        if (temp == 0) {          /* handle the unlikely case of a_val == 0 */
            maskA = 1u << 0;
        } else {
            while (temp > 0) {
                unsigned int d = (unsigned int)(temp % 10);
                maskA |= 1u << d;
                temp /= 10;
            }
        }

        /* check whether any digit of b_val appears in maskA */
        idx_t tempb = b_val;
        int ok = 1;
        if (tempb == 0) { /* b_val == 0 only possible when n == 0 which is excluded by loop */
            if (maskA & 1u) ok = 0;
        } else {
            while (tempb > 0 && ok) {
                unsigned int d = (unsigned int)(tempb % 10);
                if (maskA & (1u << d)) {
                    ok = 0;
                }
                tempb /= 10;
            }
        }

        if (ok) {
            ++count;
        }
    }
    return count;
}
