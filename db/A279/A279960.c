/*
 * A279960  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 221", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A279960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279960: binary representation of the x‑axis of Rule 221
   closed form:
   a(0)=1, a(1)=1, a(2)=10, a(3)=11
   for even n≥4   a(n) = 111 · 10^(n-2)
   for odd  n≥3   a(n) = (10^(n-1) – 1)/9   (i.e. a string of n‑1 ones)
   (equivalently: a(n) = ((989*(-10)^n + 1009*10^n) + (n odd ? -200 : 0)) / 1800)
*/

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1)
        return seq_from_str("1");
    if (n == 2)
        return seq_from_str("10");
    if (n == 3)
        return seq_from_str("11");

    if ((n & 1) == 0) {                /* even n ≥ 4 */
        idx_t zeros = n - 2;
        idx_t len   = 3 + zeros;        /* '111' plus zeros */
        char *buf = (char *)malloc((size_t)len + 1);
        if (!buf) return (seq_t)0;      /* allocation failed */

        memcpy(buf, "111", 3);
        memset(buf + 3, '0', zeros);
        buf[len] = '\0';

        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    } else {                            /* odd n ≥ 5 */
        idx_t ones = n - 1;              /* number of '1'‑digits */
        idx_t len  = ones;
        char *buf = (char *)malloc((size_t)len + 1);
        if (!buf) return (seq_t)0;

        memset(buf, '1', ones);
        buf[len] = '\0';

        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    }
}
