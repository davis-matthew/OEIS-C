/*
 * A255818  a(n) = 2*B*C*(n mod A) + A*C*(n mod B) + A*B*(n mod C) with A=3,
 *          B=5, C=7.
 * https://oeis.org/A255818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*B*C*(n mod A) + A*C*(n mod B) + A*B*(n mod C) with A =
 *            3, B = 5, C = 7. This expands to a(n) = 70*(n mod 3) + 21*(n mod
 *            5) + 15*(n mod 7)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based; the formula works directly with n. */
    seq_t term = ((seq_t)(n % 3u) * 70u)   /* 2*B*C = 70 */
               + ((seq_t)(n % 5u) * 21u)   /* A*C   = 21 */
               + ((seq_t)(n % 7u) * 15u);  /* A*B   = 15 */
    return term;
}
