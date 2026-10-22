/*
 * A308225  Take the integer-sided triangles with perimeter n and mutually
 *          coprime sides a, b and c such that a <= b <= c. a(n) is the sum of
 *          all the b's.
 * https://oeis.org/A308225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all middle sides b of mutually coprime
 *            integer-sided triangles with perimeter n (a ≤ b ≤ c). Triangle
 *            inequality reduces to a + b > n/2
 * Range:     u8 n <= 47, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Euclidean algorithm for idx_t values. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t a, b, c;
    idx_t half = n / 2;

    for (a = 1; a <= n / 3; ++a) {
        /* b must satisfy a ≤ b ≤ c and a + b > c.  The latter yields 
           b > n/2 - a. */
        idx_t low_b = half - a + 1;
        if (low_b < a) low_b = a;
        idx_t high_b = (n - a) / 2;
        if (low_b > high_b) continue;

        for (b = low_b; b <= high_b; ++b) {
            c = n - a - b;
            if (gcd_idx(a, b) != 1) continue;
            if (gcd_idx(a, c) != 1) continue;
            if (gcd_idx(b, c) != 1) continue;
            sum += (seq_t)b;
        }
    }
    return sum;
}
