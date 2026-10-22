/*
 * A339858  Middle side of integer-sided primitive triangles whose sides a < b
 *          < c form a geometric progression.
 * https://oeis.org/A339858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = middle side of integer‑sided primitive triangles whose sides
   a < b < c form a geometric progression.
   The triples are enumerated by coprime pairs (x,y) with x<y,
   satisfying x^2 + x*y > y^2 (triangle inequality).
   For each such pair, the primitive triple is
       a = x^2,   b = x*y,   c = y^2
   Triangles are listed in increasing lexicographic order of (a,b,c),
   so the b's appear in the order generated below. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but return 0 safely */

    idx_t count = 0;              /* number of triples found so far */

    for (idx_t x = 2; ; ++x)      /* iterate over x (the smaller side's sqrt) */
    {
        /* for each x, scan all y> x that satisfy the triangle inequality */
        for (idx_t y = x + 1; ; ++y)
        {
            /* use seq_t arithmetic to avoid overflow for small x,y */
            seq_t lhs = (seq_t)x * (seq_t)x + (seq_t)x * (seq_t)y;  /* a + b */
            seq_t rhs = (seq_t)y * (seq_t)y;                       /* c   */

            if (lhs <= rhs)                     /* no longer a triangle */
                break;

            if (gcd((seq_t)x, (seq_t)y) == 1)   /* primitive condition */
            {
                ++count;
                if (count == n)
                    return (seq_t)x * (seq_t)y;  /* middle side b = x*y */
            }
        }
    }
}
