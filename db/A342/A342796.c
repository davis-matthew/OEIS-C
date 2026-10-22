/*
 * A342796  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to the same vertex in the 6-point
 *          set.
 * https://oeis.org/A342796
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(4) = 709682, a(5) = 77784248, a(6) = 6126191066, a(7) =
 *            427218509360, a(8) = 28245026082242, a(9) = 1821452259070568,
 *            a(n) = 120*a(n-1) - 4593*a(n-2) + 69688*a(n-3) - 428787*a(n-4) +
 *            978768*a(n-5) - 615195*a(n-6) for n > 9
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    if (n == 4) return 709682;
    if (n == 5) return 77784248;
    if (n == 6) return 6126191066;
    if (n == 7) return 427218509360;
    if (n == 8) return 28245026082242;
    if (n == 9) return 1821452259070568;

    /* Values stored as a(n-1), a(n-2), ..., a(n-6) */
    seq_t val[6];
    val[0] = 1821452259070568; /* a9  */
    val[1] = 28245026082242;   /* a8  */
    val[2] = 427218509360;     /* a7  */
    val[3] = 6126191066;       /* a6  */
    val[4] = 77784248;         /* a5  */
    val[5] = 709682;           /* a4  */

    for (idx_t i = 10; i <= n; ++i) {
        /* Compute next term using the linear recurrence */
        seq_t term0 = val[0] * 120;   /* 120 * a(n-1) */
        seq_t term1 = val[1] * 4593;  /* 4593 * a(n-2) */
        seq_t term2 = val[2] * 69688; /* 69688 * a(n-3) */
        seq_t term3 = val[3] * 428787;/* 428787 * a(n-4) */
        seq_t term4 = val[4] * 978768;/* 978768 * a(n-5) */
        seq_t term5 = val[5] * 615195;/* 615195 * a(n-6) */

        seq_t next = term0 + term2 + term4 - term1 - term3 - term5;

        /* Shift the window for the next iteration */
        val[5] = val[4];
        val[4] = val[3];
        val[3] = val[2];
        val[2] = val[1];
        val[1] = val[0];
        val[0] = next;
    }

    return val[0];
}
