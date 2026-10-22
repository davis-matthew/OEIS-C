/*
 * A308091  Sum of the areas of the integer-sided triangles with perimeter n
 *          and integer area.
 * https://oeis.org/A308091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over all integer-sided triangles with perimeter n and
 *            * integer area of that area. Trivial: a(odd)=0, as a
 *            half-integer semiperimeter * cannot give an integer area. For
 *            even n compute all triples a<=b<=c, * a+b>c, a+b+c=n, and add
 *            sqrt(s(s-a)(s-b)(s-c)) when it is integral
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides extern seq_t floor_sqrt(seq_t); */

seq_t a(idx_t n)
{
    /* For odd perimeters the semiperimeter is half-integer; no integer area exists. */
    if (n & 1ULL) return 0;

    idx_t s = n >> 1;            /* semiperimeter, integer */

    seq_t sum = 0;
    /* a ranges from 1 to n/3 (since a <= b <= c and a+b+c=n). */
    for (idx_t a = 1; a <= n / 3; ++a) {
        /* b ranges from a to floor((n-a)/2) to ensure b <= c. */
        idx_t max_b = (n - a) >> 1;
        for (idx_t b = a; b <= max_b; ++b) {
            if (a + b <= s) continue;          /* triangle inequality a+b>c == a+b>n/2 */
            idx_t c = n - a - b;
            /* Compute area^2 = s*(s-a)*(s-b)*(s-c), all integers for even n. */
            seq_t area2 = (seq_t)s * (seq_t)(s - a) *
                          (seq_t)(s - b) * (seq_t)(s - c);
            seq_t sq = floor_sqrt(area2);
            if (sq * sq == area2) {
                sum += sq;   /* area is an integer */
            }
        }
    }
    return sum;
}
