/*
 * A308308  Number of integer-sided triangles with perimeter n and at least
 *          one pair of side lengths that are not coprime.
 * https://oeis.org/A308308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A005044(n) - A308074(n) (Number of integer-sided
 *            triangles with perimeter n that have at least one pair of side
 *            lengths not coprime)
 * Range:     u8 n <= 116, u16 n <= 1900, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;        /* Perimeter < 3 cannot form a triangle */

    seq_t count = 0;
    idx_t max_a = n / 3;        /* a <= floor(n/3) */

    for (idx_t a = 1; a <= max_a; ++a) {
        /* b must satisfy a <= b <= floor((n-a)/2)   (so b <= c)
           and a + b > n/2  (triangle inequality)    */
        idx_t max_b = (n - a) / 2;
        idx_t min_b = a;                         /* start at a */

        /* compute lower bound for b from a+b > n/2  ->  b > n/2 - a  */
        idx_t half = n / 2;                      /* integer division */
        idx_t lb = half > a ? half - a + 1 : a + 1; /* avoid underflow */
        if (lb > min_b) min_b = lb;

        if (max_b < min_b) continue;             /* no valid b for this a */

        for (idx_t b = min_b; b <= max_b; ++b) {
            idx_t c = n - a - b;                  /* side c */

            /* Check pairwise coprimeness */
            seq_t g1 = gcd((seq_t)a, (seq_t)b);
            if (g1 != 1) { ++count; continue; }

            seq_t g2 = gcd((seq_t)a, (seq_t)c);
            if (g2 != 1) { ++count; continue; }

            seq_t g3 = gcd((seq_t)b, (seq_t)c);
            if (g3 != 1) { ++count; continue; }

            /* All three sides coprime -> skip */
        }
    }

    return count;
}
