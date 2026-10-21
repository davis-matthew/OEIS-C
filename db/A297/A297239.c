/*
 * A297239  Total variation of base-13 digits of n; see Comments.
 * https://oeis.org/A297239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of absolute differences of successive base‑13 digits
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Extract base‑13 digits from least to most significant.  The sum of
       absolute differences is independent of the order of the digits, so we
       can accumulate the result on the fly without storing the whole digit
       sequence. */
    idx_t v = n;                 /* working copy of the index */
    int   prev = -1;             /* previous digit, -1 means none yet */
    seq_t sum = 0;               /* running total of variations */

    while (v > 0) {
        int curr = v % 13;      /* current least‑significant digit */
        v = v / 13;             /* shift right by one base‑13 digit */

        if (prev != -1) {       /* at least two digits seen so far */
            int diff = curr > prev ? curr - prev : prev - curr;
            sum += (seq_t)diff;
        }
        prev = curr;            /* store current digit for next iteration */
    }

    return sum;
}
