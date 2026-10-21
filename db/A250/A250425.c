/*
 * A250425  Number of (n+1) X (n+1) 0..1 arrays with nondecreasing sum of
 *          every two consecutive values in every row and column.
 * https://oeis.org/A250425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = [C(n+1, ⌊(n+1)/2⌋)]^4 for odd n * a(n) = [C(n+1,
 *            ⌊(n+1)/2⌋)]^4 · (n+2)/(n+1) for even n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t k = (n + 1) / 2;          /* floor((n+1)/2) */
    seq_t b = binomial(n + 1, k);   /* choose base */

    /* compute b^4 */
    seq_t res = b;
    res *= b;                     /* b^2 */
    res *= b;                     /* b^3 */
    res *= b;                     /* b^4 */

    /* adjust for even n */
    if ((n & 1) == 0) {
        res *= (seq_t)(n + 2);
        res /= (seq_t)(n + 1);    /* divisions are exact */
    }
    return res;
}
