/*
 * A329506  Expansion of (1 + x)*(1 + 2*x + 2*x^2 - 2*x^3) / (1 - x).
 * https://oeis.org/A329506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 3*I[n≥1] + 4*I[n≥2] - 2*I[n≥4]
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A329506. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    if (n < 4)  /* i.e., n == 2 or n == 3 */
        return (seq_t)8;
    return (seq_t)6;   /* n >= 4 */
}
