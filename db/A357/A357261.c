/*
 * A357261  a(n) is the number of blocks in the bottom row after adding n
 *          blocks to the preceding structure of rows. See Comments and
 *          Example sections for more details.
 * https://oeis.org/A357261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = number of blocks in the bottom row after adding n blocks
 *            * according to the algorithm described in the OEIS entry
 *            A357261. * The algorithm keeps a current row width starting at
 *            3. * For each addition i = 1..n, exactly i blocks are inserted.
 *            * Whenever a row becomes full during an addition...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard against zero */

    /* current_width: width of the row we are filling (for next blocks)   */
    idx_t current_width = 3;
    /* current_pos: number of blocks already placed in the current row   */
    idx_t current_pos   = 0;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t remain = i;           /* i blocks to add for this step */

        /* Insert the i blocks following the described rules.            */
        while (remain > 0) {
            idx_t space = current_width - current_pos;   /* free cells **/

            if (remain < space) {        /* all remaining blocks fit here */
                current_pos += remain;
                remain = 0;
            } else {                      /* fill current row exactly  */
                current_pos = current_width;
                remain -= space;
                if (remain > 0) {         /* still blocks left; start new row */
                    current_pos = 0;
                } else {                  /* addition finished exactly on a row border */
                    current_width += 1;    /* width for next addition increases */
                    current_pos = 0;
                }
            }
        }

        /* Bottom row length after this addition:
           if the current row was just finished exactly, current_pos==0
           and current_width has already been incremented.                     */
        seq_t bottom_len;
        if (current_pos == 0) {
            bottom_len = (seq_t)(current_width - 1); /* previous width */
        } else {
            bottom_len = (seq_t)current_pos;
        }

        if (i == n) return bottom_len;
        /* current_pos and current_width already hold the correct state
           for the start of the next addition. */
    }

    return 0;   /* unreachable, but keeps compiler happy */
}
