/*
 * A340279  Number of partitions of n into 3 parts such that the smallest part
 *          is relatively prime to at least one other part.
 * https://oeis.org/A340279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into 3 parts such that the smallest
 *            part is relatively prime to at least one other part. Implements
 *            the definition by iterating over all unordered partitions (a ≤ b
 *            ≤ c) with a + b + c = n and counting those where gcd(a, b) = 1
 *            or gcd(a, c) = 1
 * Range:     u8 n <= 58, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t max_a = n / 3;           /* smallest part cannot exceed floor(n/3) */

    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t max_b = (n - a) / 2; /* ensure b ≤ c and a ≤ b */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;  /* c ≥ b automatically by construction */

            /* Count if a and b are coprime, or a and c are coprime. */
            if ((idx_t)gcd((seq_t)a, (seq_t)b) == 1 ||
                (idx_t)gcd((seq_t)a, (seq_t)c) == 1) {
                sum += 1;
            }
        }
    }
    return sum;
}
