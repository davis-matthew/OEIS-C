/*
 * A306488  Number of ways of expressing n as a + b + c, with a, b, and c
 *          positive integers, gcd(a, b) = 1, but gcd(a, c) and gcd(b, c) both
 *          greater than 1.
 * https://oeis.org/A306488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into three positive
 *            integers a, b, c such that one can label two of the parts as x,
 *            y with gcd(x, y) = 1 and the remaining part z satisfies gcd(x,
 *            z) > 1 and gcd(y, z) > 1. The counting is done over unordered
 *            partitions (i.e. a ≤ b ≤ c) and a partition i...
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* n less than 3 cannot be split into three positive parts */
    if (n < 3) return 0;

    seq_t count = 0;

    /* iterate over unordered triples a ≤ b ≤ c */
    for (idx_t a = 1; a <= n/3; ++a) {
        idx_t max_b = (n - a) / 2;          /* ensures b ≤ c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (c < b) continue;           /* safety, should not happen */

            /* test all three possible labelings */
            bool ok = false;

            /* labeling (a, b) as the coprime pair */
            if (gcd((seq_t)a, (seq_t)b) == 1 &&
                gcd((seq_t)a, (seq_t)c) > 1 &&
                gcd((seq_t)b, (seq_t)c) > 1) {
                ok = true;
            }
            /* labeling (a, c) as the coprime pair */
            else if (gcd((seq_t)a, (seq_t)c) == 1 &&
                     gcd((seq_t)a, (seq_t)b) > 1 &&
                     gcd((seq_t)c, (seq_t)b) > 1) {
                ok = true;
            }
            /* labeling (b, c) as the coprime pair */
            else if (gcd((seq_t)b, (seq_t)c) == 1 &&
                     gcd((seq_t)b, (seq_t)a) > 1 &&
                     gcd((seq_t)c, (seq_t)a) > 1) {
                ok = true;
            }

            if (ok) ++count;
        }
    }

    return count;
}
