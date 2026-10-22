/*
 * A317483  Circuit rank of the n-Bruhat graph.
 * https://oeis.org/A317483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*n!/2 + 1 Computes the circuit rank of the n‑Bruhat
 *            graph (OEIS A317483)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Special cases where (n-3) would be negative. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* Compute n! */
    seq_t fac = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fac *= (seq_t)i;
    }

    /* Compute (n-3)*n!/2 + 1 using unsigned arithmetic. */
    seq_t prod = fac * (((seq_t)n) - 3);
    seq_t res  = prod / 2 + 1;
    return res;
}
