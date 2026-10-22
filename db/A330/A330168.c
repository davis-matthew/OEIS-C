/*
 * A330168  Length of the longest run of 2's in the ternary expression of n.
 * https://oeis.org/A330168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest run of 2's in ternary representation of
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;          /* working copy of n */
    seq_t max_run = 0;    /* longest run found so far */
    seq_t curr_run = 0;   /* current run length */

    while (x > 0) {
        if ((x % 3) == 2) {
            curr_run++;               /* extend current run of 2's */
        } else {
            if (curr_run > max_run)   /* end of a run */
                max_run = curr_run;
            curr_run = 0;             /* reset for next run */
        }
        x /= 3;                      /* move to next ternary digit */
    }

    if (curr_run > max_run)           /* handle final run */
        max_run = curr_run;

    return max_run;
}
