/*
 * A257261  One-based position of the rightmost one in the factorial base
 *          representation (A007623) of n, 0 if no one is present.
 * https://oeis.org/A257261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the position (1‑based, least significant digit has index
 *            1) of the rightmost digit equal to 1 in the factorial base
 *            (A007623) representation of n; it is 0 if the representation
 *            contains no 1
 * Range:     u8 n <= 9972, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t pos = 1;          /* current factorial-base digit position (1‑based) */
    while (n > 0)           /* iterate until all factorial digits are processed */
    {
        if (n % (pos + 1) == 1)     /* rightmost 1 found (least significant first) */
            return (seq_t)pos;      /* return its 1‑based position */
        n /= (pos + 1);             /* advance to next higher factorial digit */
        pos++;                      /* increment position counter */
    }
    return 0;                       /* no 1 in the representation */
}
