/*
 * A269772  Number of length-n 0..4 arrays with every repeated value unequal
 *          to the previous repeated value plus one mod 4+1.
 * https://oeis.org/A269772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 12*a(n-2) - 16*a(n-3) for n >= 4 Initial
 *            terms: a(1) = 5, a(2) = 25, a(3) = 125
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence grows very rapidly; the recurrence uses only
       additions, subtractions, and multiplications by small constants.
       All intermediate values fit into the arbitrary‑precision
       'seq_t' type. */
    seq_t a1 = 5;   /* a(1) */
    seq_t a2 = 25;  /* a(2) */
    seq_t a3 = 125; /* a(3) */
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    seq_t a4;
    for (idx_t i = 4; i <= n; ++i) {
        a4 = 8 * a3 - 12 * a2 - 16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
