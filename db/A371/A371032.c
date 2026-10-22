/*
 * A371032  a(n) is the integer whose decimal digits are 0's or 1's in
 *          alternating runs of lengths n, n-1, n-2, ..., 3, 2, 1.
 * https://oeis.org/A371032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the decimal integer whose digits consist of alternating
 *            runs of 1's and 0's of lengths n, n‑1, … , 2, 1, starting with a
 *            run of 1's. No closed‑form is used; we build the number digit by
 *            digit
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;          /* accumulator for the number   */
    int digit = 1;          /* first run is made of '1's    */

    /* work from the longest run (length n) down to the single
       last digit.  For each run we append 'len' copies of the
       current digit, then switch to the other digit.    */
    for (idx_t len = n; len != 0; --len) {
        for (idx_t i = 0; i < len; ++i) {
            val = val * 10 + (seq_t)digit;
        }
        digit ^= 1;   /* toggle between 1 and 0 */
    }
    return val;
}
