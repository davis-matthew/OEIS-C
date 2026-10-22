/*
 * A350236  a(n) is the sum of the entries in an n X n X n 3D matrix whose
 *          elements start at 1 in the corner cells and increase by 1 with
 *          each step towards the center.
 * https://oeis.org/A350236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/4)*n^2 * (n^2 - (2/3)*n + (n mod 2))
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The integer form is a(n) = (3*n^4 - 2*n^3 + 3*(n%2)*n^2) / 4 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use 64‑bit intermediate arithmetic to avoid overflow on 32‑bit builds.
       The result fits in a 64‑bit unsigned integer for all n <= 10000. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long n2 = nn * nn;            /* n^2 */
    unsigned long long n3 = n2 * nn;            /* n^3 */
    unsigned long long n4 = n2 * n2;            /* n^4 */
    unsigned long long m  = nn & 1ull;          /* n mod 2 (0 or 1) */

    unsigned long long numerator = 3ull * n4
                                  - 2ull * n3
                                  + 3ull * m * n2;

    unsigned long long result = numerator / 4ull;
    return (seq_t)result;
}
