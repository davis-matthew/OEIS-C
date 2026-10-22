/*
 * A341673  Irregular triangle read by rows giving the strictly superior
 *          divisors of n.
 * https://oeis.org/A341673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th strictly superior divisor when all rows of the
 *            irregular triangle are concatenated. A strictly superior divisor
 *            of a number m is a divisor d with d > m/d. The triangle lists
 *            these divisors in increasing order for each row
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0; avoid undefined behavior */
        return 0;

    idx_t cumulative = 0;  /* how many divisors have been seen so far */

    for (idx_t row = 1; ; ++row) {
        /* collect strictly superior divisors of 'row' */
        seq_t vals[512];
        idx_t len = 0;

        for (idx_t d = 1; d <= row / d; ++d) {
            if (row % d == 0) {
                idx_t d2 = row / d;
                if (d < d2) {                  /* strictly superior */
                    vals[len++] = (seq_t)d2;    /* store in descending order */
                }
            }
        }

        if (len == 0)                     /* row has no strictly superior divisors */
            continue;

        if (cumulative + len >= n) {      /* the desired index lies in this row */
            idx_t t = n - cumulative;     /* 1-based position within the row */
            idx_t idx = len - t;          /* reverse to get ascending order */
            return vals[idx];
        }

        cumulative += len;                /* move to next row */
    }
}
