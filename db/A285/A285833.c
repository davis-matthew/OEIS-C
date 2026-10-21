/*
 * A285833  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 107", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; if n is odd, a(n) is the decimal integer whose decimal
 *            digits are (n+1) ones; if n is even: n==0 → "1"; otherwise let
 *            k=n/2. If k<=3, a(n)="11" followed by k zeros (two 1's); else
 *            a(n) consists of k ones followed by k zeros
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    idx_t w, z;  /* counts of '1' and '0' digits */

    if (n & 1U) {                      /* odd n */
        w = n + 1;
        z = 0;
    } else {                           /* even n */
        if (n == 0) {                  /* special case n=0 */
            w = 1;
            z = 0;
        } else {
            idx_t half = n >> 1;       /* n/2 */
            if (half <= 3) {          /* n=2,4,6  -> two 1's, half 0's */
                w = 2;
                z = half;
            } else {                  /* n>=8 -> half 1's, half 0's */
                w = half;
                z = half;
            }
        }
    }

    size_t len = (size_t)(w + z);
    char *buf = (char*)malloc(len + 1);
    if (!buf) return 0;                /* memory allocation failed */

    for (size_t i = 0; i < w; ++i) buf[i] = '1';
    for (size_t i = 0; i < z; ++i) buf[w + i] = '0';
    buf[len] = '\0';

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
