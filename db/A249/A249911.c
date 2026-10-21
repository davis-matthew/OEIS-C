/*
 * A249911  60-gonal (hexacontagonal) numbers: a(n) = n(29n - 28).
 * https://oeis.org/A249911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(29*n - 28)
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert the index to the sequence type */
    seq_t N = (seq_t)n;
    /* Compute 29*n - 28 safely using seq_t arithmetic */
    seq_t temp = 29 * N - 28;
    return N * temp;
}
