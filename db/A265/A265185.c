/*
 * A265185  Non-vanishing traces of the powers of the adjacency matrix for the
 *          simple Lie algebra B_4: 2 * ((2 + sqrt(2))^n + (2 - sqrt(2))^n).
 * https://oeis.org/A265185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ((2 + sqrt(2))^n + (2 - sqrt(2))^n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     Recurrence: a(0) = 4, a(1) = 8, a(n) = 4*a(n-1) - 2*a(n-2)   */
seq_t a(idx_t n)
{
    /* Handle the two base cases. */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)8;

    /* Iterative computation of the linear recurrence. */
    seq_t a0 = (seq_t)4;          /* a(0) */
    seq_t a1 = (seq_t)8;          /* a(1) */
    seq_t an;                     /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)4 - a0 * (seq_t)2;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
