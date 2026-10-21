/*
 * A255108  Number of length n+1 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5)/120 + (n^4)/6 + (19 n^3)/24 + (11 n^2)/6 + (16 n)/5
 *            + 3
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: index 1 gives first term */
    if (n == 0) return 0;

    seq_t N = n;                     /* promote index to seq_t */
    __int128 nn   = (__int128)N;     /* intermediate 128‑bit arithmetic */
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;

    __int128 total = n5         /* n^5/120  -> n5              */
                   + n4 * 20    /* *20/120 = 1/6            */
                   + n3 * 95    /* *95/120 = 19/24         */
                   + n2 * 220   /* *220/120 = 11/6          */
                   + nn * 384   /* *384/120 = 16/5          */
                   + 360;       /* *360/120 = 3             */
    total /= 120;                /* divide by 120 to finish */

    return (seq_t)total;
}
