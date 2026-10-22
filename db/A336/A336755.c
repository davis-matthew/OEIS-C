/*
 * A336755  Primitive triples for integer-sided triangles whose sides a < b <
 *          c are in arithmetic progression.
 * https://oeis.org/A336755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each integer middle side b = a + d starting from 3, generate all
   integer-sided triangles (a,b,c) with sides in arithmetic progression.
   The sides are (a, b, 2b - a) where a > b/2 and a < b.
   Output the sides consecutively: a, then b, then c.  Only primitive
   triangles (gcd(a,b,c)=1) are included.  This produces the OEIS
   sequence A336755. */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    idx_t pos = n;          /* 1‑based position in the output sequence */
    idx_t count = 0;        /* number of terms output so far */

    seq_t b = 3;            /* middle side starts at 3 (the first primitive triangle is 2,3,4) */

    for (;;) {              /* iterate b until we reach the desired term */
        /* smallest possible a: floor(b/2)+1 */
        seq_t a_min = (b >> 1) + 1;

        for (seq_t a = a_min; a < b; ++a) {
            /* third side */
            seq_t c = (b << 1) - a;

            /* check primitivity */
            seq_t g = gcd(a, b);
            g = gcd(g, c);
            if (g != (seq_t)1)
                continue;

            /* emit a */
            ++count;
            if (count == pos) return a;

            /* emit b */
            ++count;
            if (count == pos) return b;

            /* emit c */
            ++count;
            if (count == pos) return c;
        }
        ++b;
    }
}
