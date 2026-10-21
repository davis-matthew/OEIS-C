/*
 * A292360  a(n) = n*(Lucas(n)*Lucas(n+1) - 2).
 * https://oeis.org/A292360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (Lucas(n) * Lucas(n+1) - 2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute Lucas numbers L(0) and L(1) */
    seq_t L0 = (seq_t)2;
    seq_t L1 = (seq_t)1;
    idx_t k;

    /* Iterate to obtain L(n) and L(n+1) */
    for (k = 0; k < n; ++k) {
        seq_t L_next = L0 + L1;
        L0 = L1;
        L1 = L_next;
    }

    /* Now L0 = Lucas(n), L1 = Lucas(n+1) */
    seq_t prod   = L0 * L1;
    seq_t result = (seq_t)n * (prod - (seq_t)2);
    return result;
}
