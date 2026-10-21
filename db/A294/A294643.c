/*
 * A294643  Length (= size) of the orbit of n under the "3x+1" map A006370: x
 *          -> x/2 if even, 3x+1 if odd. a(n) = -1 in case the orbit would be
 *          infinite.
 * https://oeis.org/A294643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of the distinct-orbit under the 3x+1 map. The orbit
 *            is counted until a repetition occurs, which in practice happens
 *            at the cycle 1 -> 4 -> 2 -> 1. For n = 0 the orbit is {0}
 * Range:     u8 n <= 26, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* 0 maps to itself */

    seq_t x = (seq_t)n;            /* current value in the orbit */
    seq_t seen = 0;                /* bit flags: 1,2,4 for 1,2,4 being visited */
    seq_t count = 0;               /* distinct values counted so far */

    for (;;)
    {
        /* Check for the cycle nodes 1, 2, 4.  Once one of them is seen
           again the orbit repeats. */
        if (x == (seq_t)1)
        {
            if (seen & 1) break;
            seen |= 1;
        }
        else if (x == (seq_t)2)
        {
            if (seen & 2) break;
            seen |= 2;
        }
        else if (x == (seq_t)4)
        {
            if (seen & 4) break;
            seen |= 4;
        }

        ++count;                    /* count this distinct value */

        /* Next value in Collatz rule */
        if ((x & 1) == 0)           /* even */
            x = x / 2;
        else                        /* odd */
            x = x * 3 + 1;
    }

    return count;
}
