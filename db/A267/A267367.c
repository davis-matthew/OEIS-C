/*
 * A267367  Decimal representation of the middle column of the "Rule 126"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=2*a(n-1)+1 if (n+1) is a power of 2,
 *            otherwise a(n)=2*a(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= n; ++i) {
        val <<= 1;                     /* multiply by 2 */
        if (((i + 1) & i) == 0)        /* n+1 is a power of 2? */
            ++val;                     /* add 1 */
    }
    return val;
}
