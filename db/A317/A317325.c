/*
 * A317325  Multiples of 25 and odd numbers interleaved.
 * https://oeis.org/A317325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 25*n, a(2n+1) = 2*n + 1
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A317325. */
seq_t a(idx_t n) {
    if (n & 1) {                     /* odd index: a = n */
        return (seq_t)n;
    } else {                         /* even index: a = 25*(n/2) */
        idx_t half = n >> 1;
        return (seq_t)half * (seq_t)25;
    }
}
