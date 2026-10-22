/*
 * A308318  Number of integer-sided triangles with perimeter n and at least
 *          one nonprime side length.
 * https://oeis.org/A308318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‐sided triangles with perimeter n that
 *            have at least one side that is not a prime number. The sequence
 *            is computed by brute–force enumeration of all unordered triples
 *            (a,b,c) of positive integers satisfying a + b + c = n, a ≤ b ≤
 *            c, and the triangle inequality a + b > c. F...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Ensure that n >= 1. If n < 3 then there can be no triangle. */
    if (n < 3) return (seq_t)0;

    seq_t total = 0;
    seq_t prime_tri = 0;

    for (idx_t a = 1; a <= n / 3; ++a) {
        /* For a fixed a, b ranges from a up to the floor((n - a)/2).
           This guarantees a ≤ b ≤ c with c = n - a - b. */
        idx_t max_b = (n - a) / 2;
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            /* Triangle inequality: a + b > c */
            if (a + b <= c) continue;

            /* Count this triangle. */
            total++;

            /* If all three sides are prime, subtract it. */
            if (is_prime(a) && is_prime(b) && is_prime(c))
                prime_tri++;
        }
    }
    return total - prime_tri;
}
