/*
 * A260033  Number of configurations of the general monomer-dimer model for a
 *          2 X 2n square lattice.
 * https://oeis.org/A260033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 7*a(n-2) + a(n-3) for n>=3, with a(0)=1,
 *            a(1)=7, a(2)=71. This follows from the generating function
 *            (1-4*x+x^2)/(1-11*x+7*x^2-x^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)71;

    /* Keep the last three values for the recurrence. */
    seq_t prev3 = (seq_t)1;   /* a(n-3) */
    seq_t prev2 = (seq_t)7;   /* a(n-2) */
    seq_t prev1 = (seq_t)71;  /* a(n-1) */
    seq_t next = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 11*a(i-1) - 7*a(i-2) + a(i-3) */
        next = (seq_t)11 * prev1 + prev3 - (seq_t)7 * prev2;
        /* Shift the window forward. */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;  /* a(n) now stored in prev1. */
}
