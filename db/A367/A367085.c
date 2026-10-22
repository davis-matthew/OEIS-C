/*
 * A367085  3-valuation r(n) of the terms A367083(A367084(n)+1) = 3^r(n) which
 *          are the odd terms preceded by another odd term.
 * https://oeis.org/A367085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 5*n - 2*⌊n/11⌋ - ( (n mod 11) > 0 ) - ( (n mod 11) >
 *            5 ) where n is the 0‑based index. This follows from the diff
 *            pattern 4,5,5,5,5,4,5,... with runs of five‑differences of
 *            lengths 4,5,4,5,
 * Range:     u8 n <= 52, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* number of full [4,5⁴,4,5⁵] cycles */
    idx_t pairs = n / 11;
    /* remaining positions in the current cycle */
    idx_t r = n % 11;

    /* count of 4‑differences up to index n-1 */
    seq_t fours = ((seq_t)pairs << 1)          /* 2 per full cycle   */
                + (seq_t)(r > 0)             /* add one if remainder >0  */
                + (seq_t)(r > 5);            /* add another if remainder >5 */

    /* a(n) = 1 + 5*n + (n - fours) = 1 + 5*n - fours */
    return (seq_t)1 + ((seq_t)5 * (seq_t)n) - fours;
}
