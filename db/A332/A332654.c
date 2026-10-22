/*
 * A332654  a(n) = Sum_{k=1..n} (k/gcd(n, k))^2.
 * https://oeis.org/A332654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k/gcd(n, k))^2 Computes the sequence by a
 *            straightforward O(n) loop
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 2346, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t N = (seq_t)n;
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)k, N);        /* gcd of k and n in seq_t */
        seq_t t = (seq_t)k / g;            /* k/g */
        sum += t * t;                      /* add (k/g)^2 */
    }
    return sum;
}
