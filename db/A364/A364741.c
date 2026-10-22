/*
 * A364741  Number of edge covers in the n-double cone graph.
 * https://oeis.org/A364741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 13*a(n-1) + 2*a(n-2) - 75*a(n-3) - 17*a(n-4)
 *                        + 38*a(n-5) - 8*a(n-6)   for n >= 6
 * Initial values (offset 0):
 *   a(0) = 0
 *   a(1) = 8
 *   a(2) = 160
 *   a(3) = 2009
 *   a(4) = 25872
 *   a(5) = 328208
 */

seq_t a(idx_t n)
{
    /* first six terms */
    static const seq_t init[6] = {
        0,  /* a(0) */
        8,  /* a(1) */
        160,/* a(2) */
        2009,/* a(3) */
        25872,/* a(4) */
        328208/* a(5) */
    };

    if (n < 6)
        return init[n];

    seq_t prev[6];
    prev[0] = init[0];
    prev[1] = init[1];
    prev[2] = init[2];
    prev[3] = init[3];
    prev[4] = init[4];
    prev[5] = init[5];

    seq_t cur = 0;
    for (idx_t i = 6; i <= n; ++i)
    {
        cur = 13 * prev[5] + 2 * prev[4] - 75 * prev[3] - 17 * prev[2]
              + 38 * prev[1] - 8 * prev[0];

        /* shift window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }

    return cur;
}
