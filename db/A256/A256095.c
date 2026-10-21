/*
 * A256095  Triangle of greatest common divisors of two triangular numbers
 *          (A000217).
 * https://oeis.org/A256095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = gcd(T(r), T(c)), where T(x) = x*(x+1)/2, and r, c satisfy
 *            n = r(r+1)/2 + c, 0 <= c <= r
 * Range:     u8 n <= 275, u16 n <= 9924, u32 n <= 9924, u64 n <= 9924,
 *            u128 n <= 9924, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static idx_t triangular_root(idx_t n) {
    /* Find the largest r such that r(r+1)/2 <= n */
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 <= n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Determine row r and column c from index n */
    idx_t r = triangular_root(n);
    idx_t c = n - r * (r + 1) / 2;

    /* Compute the triangular numbers */
    seq_t tr = (seq_t)r * ((seq_t)r + 1) / 2;
    seq_t tc = (seq_t)c * ((seq_t)c + 1) / 2;

    /* Return the gcd of the two triangular numbers */
    return gcd(tr, tc);
}
