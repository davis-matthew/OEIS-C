/*
 * A247555  A permutation of the nonnegative numbers: a(4n) = 8n, a(4n+1) = 2n
 *          + 1, a(4n+2) = 4n + 2, a(4n+3) = 8n + 4.
 * https://oeis.org/A247555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(4n) = 8n, a(4n+1) = 2n+1, a(4n+2) = 4n+2, a(4n+3) = 8n+4
 * Range:     u8 n <= 127, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A247555 – a permutation of the nonnegative integers. */

seq_t a(idx_t n) {
    idx_t q = n >> 2;          /* floor(n/4) */
    idx_t r = n & 3;           /* n mod 4   */
    switch (r) {
        case 0: return (seq_t)(q * 8);
        case 1: return (seq_t)(q * 2 + 1);
        case 2: return (seq_t)(q * 4 + 2);
        case 3: return (seq_t)(q * 8 + 4);
        default: return 0;    /* unreachable */
    }
}
