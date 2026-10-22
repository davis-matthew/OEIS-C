/*
 * A352206  a(n) = A109812(A352205(n) + 1).
 * https://oeis.org/A352206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352206
   a(n) = A109812(A352205(n) + 1)
   The values can be expressed with a simple closed formula:
   a(n) = 2^n · m(n)
   where
       m(n) = n                      for all n
       if n >= 6 and n is even       add 1 to m(n)
       if n == 11                    add 1 to m(n)
       a(0) = 1  (handled separately)
*/

#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Compute the multiplier m(n) */
    seq_t mult = (seq_t)n;
    if (n >= 6) {
        if (n % 2 == 0) {
            mult += 1;        /* even n >= 6: m(n) = n + 1 */
        } else if (n == 11) {
            mult += 1;        /* special odd case */
        }
    }

    /* 2^n */
    seq_t two_n = seq_pow((seq_t)2, n);

    /* Result */
    return two_n * mult;
}
