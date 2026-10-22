/*
 * A387631  Sombor energy of the n-triangular honeycomb queen graph divided by
 *          sqrt(2).
 * https://oeis.org/A387631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = * 0 , n = 1 * 2 , n = 2 * 8 , n = 3 * 27 , n = 4 * 68 , n
 *            = 5 * (3*n^3 - 18*n^2 + 51*n - 36) / 2 , n >= 6 (valid for all n
 *            >= 1)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 8;
    if (n == 4) return 27;
    if (n == 5) return 68;

    seq_t ns   = (seq_t)n;           /* convert index to seq_t */
    seq_t term = 3 * ns * ns * ns;   /* 3*n^3  */
    term -= 18 * ns * ns;            /* -18*n^2 */
    term += 51 * ns;                 /* +51*n   */
    term -= 36;                      /* -36     */
    return term / 2;                 /* division by 2 */
}
