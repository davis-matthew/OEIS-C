/*
 * A393554  Number of ways, up to rotations and reflections, to distribute n
 *          guards on the corners and walls of an equilateral triangle castle
 *          so that each wall has an equal number of guards.
 * https://oeis.org/A393554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n³ + 36*n² + 165*n + 824)/864 + (-1)ⁿ*(27*n +
 *            176)/864 + ((2*n² + 1) mod 3)*n/9)
 * Range:     u8 none, u16 n <= 26, u32 n <= 2249, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;                 /* n as seq_t */
    seq_t n2 = nt * nt;                  /* n² */
    seq_t n3 = n2 * nt;                  /* n³ */

    /* P = 2*n³ + 36*n² + 165*n + 824 */
    seq_t P = 2 * n3 + 36 * n2 + 165 * nt + 824;

    /* Q = 27*n + 176   (the (-1)ⁿ factor handled later) */
    seq_t Qv = 27 * nt + 176;

    /* shift = ((2*n² + 1) mod 3) * n * 96   (since 864/9 = 96) */
    seq_t modTerm = (2 * n2 + 1) % 3;
    seq_t shift = modTerm * nt * 96;

    seq_t M;
    if ((nt & 1) == 0) /* n is even -> add Q */
        M = P + Qv + shift;
    else /* n is odd -> subtract Q */
        M = P - Qv + shift;   /* P > Qv for all n≥0, so no underflow */

    return M / 864;              /* integer division gives the floor */
}
