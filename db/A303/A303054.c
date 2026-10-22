/*
 * A303054  Number of minimum total dominating sets in the n-ladder graph.
 * https://oeis.org/A303054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, for n mod 3 = 0; a(n) = ((n^2 + 13*n + 4)/18)^2, for n
 *            mod 3 = 1; a(n) = ((n + 4)/3)^2, for n mod 3 = 2
 * Range:     u8 n <= 12, u16 n <= 63, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    idx_t rem = n % 3;
    if (rem == 0) {
        return (seq_t)1;
    } else if (rem == 1) {
        seq_t t = nn * nn + (seq_t)13 * nn + (seq_t)4;
        t /= (seq_t)18;          /* divisible by 18 */
        return t * t;
    } else { /* rem == 2 */
        seq_t t = nn + (seq_t)4;
        t /= (seq_t)3;           /* divisible by 3 */
        return t * t;
    }
}
