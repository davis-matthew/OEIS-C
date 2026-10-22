/*
 * A308224  Sum of the largest sides of all integer-sided triangles with
 *          perimeter n whose side lengths are mutually coprime.
 * https://oeis.org/A308224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the largest sides of all integer‑sided triangles
 *            with perimeter n whose side lengths are mutually coprime
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 3) return 0;          /* no triangle possible */

    /* Enumerate all triples (a,b,c) with a ≤ b ≤ c, a+b>c, a+b+c=n */
    for (idx_t a = 1; 3 * a <= n; ++a) {
        idx_t max_b = (n - a) / 2;          /* ensures b ≤ c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c) continue;       /* triangle inequality fails */

            /* check mutual coprimality */
            if (gcd((seq_t)a, (seq_t)b) != 1) continue;
            if (gcd((seq_t)a, (seq_t)c) != 1) continue;
            if (gcd((seq_t)b, (seq_t)c) != 1) continue;

            sum += (seq_t)c;                /* add largest side */
        }
    }
    return sum;
}
