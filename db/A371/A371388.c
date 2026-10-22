/*
 * A371388  a(n) = 2*n*floor(n*(1 + sqrt(5))/2) - n^2. Main diagonal of
 *          A340429.
 * https://oeis.org/A371388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*floor(n*(1 + sqrt(5))/2) - n^2
 * Range:     u8 n <= 3, u16 n <= 170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* n^2 */
    seq_t n2 = n_seq * n_seq;

    /* floor(n*sqrt(5)) = floor(sqrt(5*n^2)) */
    seq_t sqrt5n = floor_sqrt((seq_t)5 * n2);

    /* floor(n*phi) = floor((n + floor(n*sqrt5))/2) */
    seq_t floor_phi = (n_seq + sqrt5n) / 2;

    /* a(n) = 2*n*floor_phi - n^2 */
    seq_t result = (seq_t)2 * n_seq * floor_phi - n2;

    return result;
}
