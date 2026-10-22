/*
 * A340761  Number of partitions of n into 4 parts whose 'middle' two parts
 *          have the same parity.
 * https://oeis.org/A340761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 positive parts a≤b≤c≤d
 *            with (b mod 2) = (c mod 2)
 * Range:     u8 n <= 39, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* a ranges from 1 up to floor(n/4) because 4*a ≤ n */
    for (idx_t a = 1; a <= n / 4; ++a) {
        idx_t rest_after_a = n - a;          /* n - a */
        idx_t b_max = rest_after_a / 3;      /* b ≤ (n - a)/3 */
        for (idx_t b = a; b <= b_max; ++b) {
            idx_t rest_after_b = rest_after_a - b;   /* n - a - b */
            idx_t high_c = rest_after_b / 2;         /* c ≤ (n - a - b)/2 */
            if (high_c < b) continue;               /* impossible range */
            idx_t low_c = b;
            /* adjust low_c to have same parity as b */
            if ((low_c & 1) != (b & 1)) ++low_c;
            if (low_c > high_c) continue;
            /* number of valid c values */
            result += (seq_t)((high_c - low_c) / 2 + 1);
        }
    }
    return result;
}
