/*
 * A267258  Decimal representation of the middle column of the "Rule 111"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2·a(n‑1) + [ (n‑1) even ? 1 : 0 ] for n ≥ 7 Initial
 *            values: a(0)=1, a(1)=3, a(2)=7, a(3)=15, a(4)=31, a(5)=63,
 *            a(6)=127
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small table of the first seven terms */
    static const seq_t first7[7] = {1, 3, 7, 15, 31, 63, 127};

    if (n < 7)
        return first7[n];

    seq_t val = first7[6];          /* a(6) = 127 */
    for (idx_t i = 7; i <= n; ++i)
    {
        val = val << 1;            /* multiply by 2 */
        if (((i - 1) & (idx_t)1) == 0) /* (i‑1) even? */
            val += 1;              /* add 1 if even */
    }
    return val;
}
