/*
 * A280136  Negative continued fraction of e (or negative continued fraction
 *          expansion of e).
 * https://oeis.org/A280136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; for m>=1 the sequence is grouped as 4m, 3, 2 repeated
 *            (4m-1) times, 3
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each group m has length 4m+2. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    /* k is the 0‑based position inside the repeating groups */
    idx_t k   = n - 2;
    idx_t cum = 0;              /* number of elements already processed */

    for (idx_t m = 1; ; ++m) {
        idx_t len = 4 * m + 2;   /* length of group m */

        if (k < cum + len) {    /* the target is in this group */
            idx_t pos = k - cum;/* position inside the group */

            if (pos == 0)              /* 4m */
                return (seq_t)(4 * m);
            if (pos == 1 || pos == len - 1) /* 3 */
                return (seq_t)3;
            return (seq_t)2;               /* all other positions are 2 */
        }

        cum += len;              /* move to next group */
    }
}
