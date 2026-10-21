/*
 * A254150  Number of independent sets in the generalized Aztec diamond
 *          E(L_5,L_{2n-1}).
 * https://oeis.org/A254150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 24*a(n-2) + 5*a(n-3) with initial values
 *            a(0)=1, a(1)=8, a(2)=73
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)73;

    /* Iterate using a sliding window of the last three values */
    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)8; /* a(n-2) */
    seq_t a2 = (seq_t)73;/* a(n-1) */
    seq_t ai = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = 12*a2 + 5*a0 - 24*a1 */
        ai = (seq_t)12 * a2 + (seq_t)5 * a0 - (seq_t)24 * a1;
        /* Slide the window */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }
    return ai;
}
