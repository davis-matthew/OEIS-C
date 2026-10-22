/*
 * A356047  The number of links of a qualifying "snake" polyline that connects
 *          the midpoints of opposite sides of the n-th regular integer
 *          hexagon that allows such a construction.
 * https://oeis.org/A356047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-2) - 15*a(n-4) + a(n-6) for n > 6, * a(1)=2,
 *            a(2)=3, a(3)=44, a(4)=45, a(5)=626, a(6)=627
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    /* Prepare the first six values. */
    seq_t a1 = 2;   /* a(1)   */
    seq_t a2 = 3;   /* a(2)   */
    seq_t a3 = 44;  /* a(3)   */
    seq_t a4 = 45;  /* a(4)   */
    seq_t a5 = 626; /* a(5)   */
    seq_t a6 = 627; /* a(6)   */

    if (n <= 6)               /* return the pre‑computed head */
    {
        switch (n) {
            case 1: return a1;
            case 2: return a2;
            case 3: return a3;
            case 4: return a4;
            case 5: return a5;
            case 6: return a6;
        }
    }

    /* Compute a(n) by iterating the recurrence. */
    idx_t i;
    for (i = 7; i <= n; ++i) {
        /* a(n) = 15*a(n-2) - 15*a(n-4) + a(n-6) */
        /* Current variables hold a(n-6)…a(n-1) */
        seq_t temp = a5 * 15 - a3 * 15;  /* (a(n-2) - a(n-4)) * 15  */
        seq_t a_next = temp + a1;        /* + a(n-6) */

        /* Shift window: a1←a2, a2←a3, …, a6←a_next */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a_next;
    }
    return a6;   /* a(n) */
}
