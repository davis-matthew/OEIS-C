/*
 * A283348  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 617", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=110, a(3)=1000. For n>3, if n is even: a(n)
 *            = (10^n-10)/9, otherwise (n odd): a(n) = (10^n-1000)/9. The
 *            implementation builds the decimal representation directly to
 *            avoid overflow when 10^n cannot be fit in seq_t
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)110;
    if (n == 3) return (seq_t)1000;

    /* For n >= 4: construct the number digit by digit.  */
    seq_t result = 0;
    idx_t ones, zeros;

    if ((n & (idx_t)1) == 0) {        /* even n */
        ones  = n - 1;                /* n-1 leading 1's */
        zeros = 1;                   /* followed by one 0 */
    } else {                          /* odd n   */
        ones  = n - 3;                /* n-3 leading 1's */
        zeros = 3;                   /* followed by three 0's */
    }

    /* Append the 1's. */
    for (idx_t i = 0; i < ones; ++i)
        result = result * (seq_t)10 + (seq_t)1;

    /* Append the trailing zeros. */
    for (idx_t i = 0; i < zeros; ++i)
        result = result * (seq_t)10;

    return result;
}
