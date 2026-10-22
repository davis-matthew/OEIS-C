/*
 * A377025  Squares and cubes that are not 6th powers.
 * https://oeis.org/A377025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer that is a perfect square or a
 *            perfect cube but not both (i.e., not a sixth power). Offset = 1
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Indices of the next square and next cube to examine.  */
    idx_t i = 1;            /* i → i² */
    idx_t j = 1;            /* j → j³ */
    idx_t count = 0;        /* how many terms we have produced */

    while (1)
    {
        /* Compute the current square and cube, doing the casts first
           so that no intermediate overflow can occur in idx_t. */
        seq_t sq = (seq_t)i * (seq_t)i;                /* i² */
        seq_t cu = (seq_t)j * (seq_t)j * (seq_t)j;     /* j³ */

        if (sq < cu)          /* the next term is a square  */
        {
            count++;
            if (count == n)
                return sq;
            i++;              /* advance to the next square */
        }
        else if (cu < sq)     /* the next term is a cube   */
        {
            count++;
            if (count == n)
                return cu;
            j++;              /* advance to the next cube  */
        }
        else                  /* sq == cu → sixth power; skip it */
        {
            i++;
            j++;
        }
    }
}
