/*
 * A287016  a(n) = smallest number k such that A071904(n) + k^2 is a perfect
 *          square.
 * https://oeis.org/A287016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k such that the n-th odd composite + k² is a
 *            perfect square. The n-th odd composite is A071904(n)
 * Range:     u8 n <= 14, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/floor_sqrt.h"

/* Returns the n-th odd composite number. */
static seq_t nth_odd_composite(idx_t n) {
    seq_t odd = 9;          /* first odd composite */
    idx_t count = 1;
    if (n == 1) return odd;
    while (count < n) {
        odd += 2;           /* only odd numbers */
        if (!isPrimeTrial(odd)) {
            ++count;
        }
    }
    return odd;
}

seq_t a(idx_t n) {
    seq_t m = nth_odd_composite(n);   /* m = A071904(n) */

    /* Minimal k is obtained by looking at factorizations m = a·b with a ≤ b, a, b odd.
       For each pair (a, b) we get k = (b - a)/2.  */
    seq_t mink = (m - 1) / 2;          /* corresponds to the factor pair (1, m) */
    seq_t root = floor_sqrt(m);
    for (seq_t d = 3; d <= root; d += 2) {
        if (m % d == 0) {
            seq_t e = m / d;          /* e = b  */
            seq_t k = (e - d) / 2;
            if (k < mink) {
                mink = k;
                if (mink == 0) break; /* can't get smaller than 0 */
            }
        }
    }
    return mink;
}
