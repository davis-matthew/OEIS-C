/*
 * A330166  Length of the longest run of 0's in the ternary expression of n.
 * https://oeis.org/A330166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest run of 0's in the ternary expression of
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* ternary of 0 is "0", longest run has length 1 */
        return (seq_t)1;

    idx_t value   = n;      /* work in native integer to avoid overflow */
    idx_t max_run = 0;
    idx_t cur_run = 0;

    while (value > 0) {
        if (value % 3 == 0)          /* zero digit */
            cur_run++;
        else {
            if (cur_run > max_run)   /* end of a run of zeros */
                max_run = cur_run;
            cur_run = 0;
        }
        value /= 3;                  /* move to next ternary digit */
    }

    if (cur_run > max_run)           /* final run may be longest */
        max_run = cur_run;

    return (seq_t)max_run;           /* return as seq_t */
}
