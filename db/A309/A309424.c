/*
 * A309424  Sum x+y of generator pairs (x, y) {x and y coprime and not both
 *          odd} of primitive Pythagorean triangles, sorted by x and y (for
 *          same x).
 * https://oeis.org/A309424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A309424.
   A(n) is the sum m+n of the nth generator pair (m,n)
   with m>n>0, gcd(m,n)=1 and m-n odd (primitive Pythagorean triples),
   sorted by increasing m and then by increasing n. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(1)=3, a(2)=5, a(3)=5, a(4)=7, ... as in the OEIS sequence. */
    idx_t count = 0;
    for (idx_t m = 2; ; ++m) {
        for (idx_t k = 1; k < m; ++k) {
            if (((m - k) & 1ULL) != 0ULL) {            /* m and n not both odd */
                if (gcd((seq_t)m, (seq_t)k) == 1) {    /* coprime */
                    ++count;
                    if (count == n) {
                        return (seq_t)(m + k);
                    }
                }
            }
        }
    }
}
