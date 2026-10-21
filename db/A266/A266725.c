/*
 * A266725  Total number of OFF (white) cells after n iterations of the "Rule
 *          59" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n/2)*(floor(n/2)+1) + (n mod 2)
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the total number of OFF cells after n iterations of Rule 59
   elementary cellular automaton starting from a single ON cell. */

seq_t a(idx_t n)
{
    /* floor division by 2 */
    seq_t m = (seq_t)(n >> 1);          /* m = n/2 (rounded down) */

    /* base value for even n: 2*m*(m+1) */
    seq_t val = m * (m + 1) * 2;

    /* if n is odd, add 1 */
    if (n & 1U)
        return val + 1;
    else
        return val;
}
