/*
 * A314969  Coordination sequence Gal.4.78.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + delta(n), where delta(n) is 1 (n=0) or depends on n
 *            mod 4 and the parity of ⌊n/4⌋. For n>0: r = n mod 4 q = n / 4
 *            r==0 : delta = 0 r==1 : delta = (q odd) ? 1 : 0 r==2 : delta =
 *            (q odd) ? 1 : -1 r==3 : delta = (q odd) ? 0 : -1 Then a(n) = 5*n
 *            + delta
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External types defined by the build. */
seq_t a(idx_t n)
{
    /* Base 5*n */
    seq_t base = 5 * (seq_t)n;
    int delta = 0;

    if (n == 0) {
        delta = 1;          /* a(0) = 1 */
    } else {
        idx_t q = n / 4;
        unsigned r = (unsigned)(n % 4);

        switch (r) {
            case 0:
                delta = 0;
                break;
            case 1:
                /* r == 1: delta = 1 if q is odd, else 0 */
                delta = (q & 1) ? 1 : 0;
                break;
            case 2:
                /* r == 2: delta = 1 if q is odd, else -1 */
                delta = (q & 1) ? 1 : -1;
                break;
            case 3:
                /* r == 3: delta = 0 if q is odd, else -1 */
                delta = (q & 1) ? 0 : -1;
                break;
        }
    }

    seq_t result = base;
    if (delta > 0) {
        result += (seq_t)delta;
    } else if (delta < 0) {
        result -= (seq_t)(-delta);
    }
    return result;
}
