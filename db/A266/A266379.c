/*
 * A266379  Binary representation of the n-th iteration of the "Rule 21"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 *          a(n) = 0              if n is even
 *          a(n) = (10^len - 1) / 9   otherwise
 *           where  len = 2   (n == 1)
 *              len = 2*n+1  (n odd, n > 1)
 * This generates the decimal integer consisting of `len` ones.
 */
seq_t a(idx_t n)
{
    if (n == 0)              /* special start value */
        return 1;

    if ((n & 1u) == 0)       /* even n -> 0 */
        return 0;

    /* odd n */
    idx_t len = (n == 1) ? 2 : 2 * n + 1;

    /* compute 10^len */
    seq_t pow10 = 1;
    seq_t ten   = 10;
    for (idx_t i = 0; i < len; ++i)
        pow10 *= ten;

    /* value made of len ones = (10^len - 1) / 9 */
    return (pow10 - 1) / 9;
}
