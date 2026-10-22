/*
 * A370110  Number of chordless cycles in the complement of the n X n antelope
 *          graph.
 * https://oeis.org/A370110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4*n^4-56*n^3+185*n^2+903*n-5142) for n > 11
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 155, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values for 1 ≤ n ≤ 11: 0,0,0,0,24,252,1032,2836,6332,12496,22328 */
seq_t a(idx_t n)
{
    if (n <= 11) {
        static const seq_t small[12] = {0, 0, 0, 0, 0, 24, 252, 1032,
                                        2836, 6332, 12496, 22328};
        return small[n];
    }
    seq_t nt  = (seq_t)n;      /* convert index to seq_t */
    seq_t n2  = nt * nt;       /* n^2 */
    seq_t n3  = n2 * nt;       /* n^3 */
    seq_t n4  = n2 * n2;       /* n^4 */

    /* Compute 4*n^4 + 185*n^2 + 903*n first (always non‑negative) */
    seq_t term = (seq_t)4 * n4 + (seq_t)185 * n2 + (seq_t)903 * nt;

    /* Subtract the 56*n^3 term (valid for n ≥ 12) */
    term -= (seq_t)56 * n3;

    /* Subtract the constant 5142 and double the result */
    term -= 5142;
    term *= 2;

    return term;
}
