/*
 * A265429  Total number of ON (black) cells after n iterations of the "Rule
 *          188" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2 + 24*n + 15 - 3*(-1)^n + 4*cos(n*pi/2)) / 16
 *            (cos(n*pi/2) = 1 if n≡0 mod 4, –1 if n≡2 mod 4, 0 otherwise)
 * Range:     u8 n <= 4, u16 n <= 139, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 6*n^2 */
    seq_t N = (seq_t)6 * (seq_t)n * (seq_t)n;
    /* + 24*n */
    N += (seq_t)24 * (seq_t)n;
    /* + 15 */
    N += (seq_t)15;

    /* subtract 3*(-1)^n  →  -3 if n even, +3 if n odd */
    if ((n & 1ULL) == 0ULL)   /* n even */
        N -= (seq_t)3;
    else                     /* n odd  */
        N += (seq_t)3;

    /* add 4*cos(n*pi/2): +4 if n≡0 mod 4, –4 if n≡2 mod 4, 0 otherwise */
    switch (n & 3ULL) {
        case 0:  N += (seq_t)4; break;
        case 2:  N -= (seq_t)4; break;
        default: /* n≡1 or 3 mod 4: add 0 */ break;
    }

    /* divide by 16 (exact division) */
    return N / (seq_t)16;
}
