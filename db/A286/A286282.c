/*
 * A286282  Stage at which Ken Knowlton's elevator (version 2) reaches floor n
 *          for the first time.
 * https://oeis.org/A286282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A286282: Stage at which Ken Knowlton's elevator (version 2) reaches floor n for the first time.
 * Recurrence: a(1)=1, a(n+1)=n*a(n)+n^2-3*n+3
 */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t cur = (seq_t)1;          /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        seq_t mi   = (seq_t)i;
        seq_t prod = mi * cur;                       /* n*a(n)   */
        seq_t sq   = mi * mi;                        /* n^2      */
        seq_t th   = ((seq_t)3) * mi;                /* 3*n      */
        cur = prod + sq - th + ((seq_t)3);           /* a(n+1)   */
    }
    return cur;  /* cur holds a(n) */
}
