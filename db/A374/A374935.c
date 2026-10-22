/*
 * A374935  Maximum number of squares covered (i.e., attacked) by 5
 *          independent (i.e., nonattacking) queens on an n X n chessboard.
 * https://oeis.org/A374935
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n^2 if n <= 11 a(n) is the maximum number of squares
 *            attacked by 5 non‑attacking queens on an n x n board. For n >=
 *            12 set t = n - 10. If t == 2 → covered squares = n^2 - 10 If t
 *            >= 3 → covered squares = n^2 - (t^2 + 8 - (t & 1)) (t & 1) is 1
 *            for odd t, 0 for even t, giving the observed pa...
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* full coverage up to n = 11 */
    if (n <= 11)
    {
        seq_t nn = (seq_t)n;
        return nn * nn;
    }

    /* more than 11 */
    idx_t t = n - 10;               /* t >= 2 */
    seq_t nn          = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t diff;

    if (t == 2U)
    {
        diff = (seq_t)10;           /* special case for n = 12 */
    }
    else
    {
        seq_t tt = (seq_t)t * (seq_t)t;          /* t^2 */
        /* t even -> +8, t odd -> +7  (8 - (t & 1)) */
        diff = tt + (seq_t)(8U - (t & 1U));
    }

    return nn - diff;
}
