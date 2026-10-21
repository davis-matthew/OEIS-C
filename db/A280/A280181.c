/*
 * A280181  Indices of centered 9-gonal numbers (A060544) that are also
 *          squares (A000290).
 * https://oeis.org/A280181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3); a(1) = 1, a(2) = 17, a(3)
 *            = 561; Valid for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 17;
    if (n == 3) return 561;
    seq_t a1 = 1;        /* a(1) */
    seq_t a2 = 17;       /* a(2) */
    seq_t a3 = 561;      /* a(3) */
    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * (seq_t)35 - a2 * (seq_t)35 + a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
