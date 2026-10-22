/*
 * A319373  a(n) = 1*2 - 3*4 + 5*6 - 7*8 + 9*10 - 11*12 + 13*14 - ... + (up to
 *          n).
 * https://oeis.org/A319373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1*2 - 3*4 + 5*6 - 7*8 + ... up to n (for an odd n, the
 *            last term is the single odd number with the sign of the missing
 *            pair). The sequence can be computed in O(n) by accumulating the
 *            alternating signed products of successive pairs
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 for safety */

    idx_t m = n >> 1;              /* floor(n/2) – number of complete pairs */
    seq_t sum = 0;
    seq_t sign = 1;                /* +1 for the first pair, then alternate */

    /* Process all complete pairs  (1*2, 3*4, 5*6, ...) */
    for (idx_t k = 1; k <= m; ++k) {
        seq_t prod = (seq_t)(2*k - 1) * (seq_t)(2*k); /* (2k-1)*(2k) */
        sum += sign * prod;
        sign = -sign;                                 /* switch sign for next pair */
    }

    /* If n is odd, add the remaining single odd number with its sign */
    if (n & 1) {
        sum += sign * (seq_t)(n);   /* n == 2m+1 */
    }

    return sum;
}
