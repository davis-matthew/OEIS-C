/*
 * A342568  1/a(n) is the current through the resistor at the central rung of
 *          an electrical ladder network made of 6*n+1 one-ohm resistors, fed
 *          by 1 volt at diametrically opposite ends of the ladder.
 * https://oeis.org/A342568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 8*a(n-1) - 18*a(n-2) + 8*a(n-3) - a(n-4);
 * a(0) = 1, a(1) = 7, a(2) = 45, a(3) = 239
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)45;
    if (n == 3) return (seq_t)239;

    /* a0..a3 correspond to a(n-4)..a(n-1) respectively. */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)7;
    seq_t a2 = (seq_t)45;
    seq_t a3 = (seq_t)239;
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = ((seq_t)8) * a3 - ((seq_t)18) * a2 + ((seq_t)8) * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
