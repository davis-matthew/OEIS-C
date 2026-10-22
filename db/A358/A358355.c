/*
 * A358355  Maximum length of an induced path (or chordless path) in the
 *          n-halved cube graph.
 * https://oeis.org/A358355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 if n = 1; 1 if n = 2 or 3; n^2 - 8n + 18 for n ≥ 4
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2 || n == 3) return 1;
    seq_t nn = (seq_t)n;
    /* compute n^2 - 8n + 18 using only integer arithmetic */
    return nn * nn - (8 * nn) + 18;
}
