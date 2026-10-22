/*
 * A336973  Sum of the smallest and largest side lengths of all distinct
 *          integer-sided triangles with perimeter n.
 * https://oeis.org/A336973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sum of the smallest and largest side lengths of all
   distinct integer‑sided triangles with perimeter n.
   The function enumerates all integer triples (a,b,c) with
   a <= b <= c, a+b>c, a+b+c=n and adds a+c for each. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;                     /* no triangle possible */

    seq_t sum = 0;
    idx_t a_limit = n / 3;              /* maximum possible value of the smallest side */

    for (idx_t a = 1; a <= a_limit; ++a) {
        idx_t max_b = (n - a) / 2;      /* largest possible middle side with b <= c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;        /* remaining side */
            if (a + b <= c)            /* triangle inequality failed */
                continue;
            sum += (seq_t)a + (seq_t)c;
        }
    }
    return sum;
}
