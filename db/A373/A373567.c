/*
 * A373567  Expansion of x + 1/(-x - 1/(-x - 1/(-x + 1))).
 * https://oeis.org/A373567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,a(1)=4,a(2)=6,a(3)=14,a(4)=31;
 * for n>=5 a(n)=2*a(n-1)+a(n-2)-a(n-3) */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 6;
    if (n == 3) return 14;
    if (n == 4) return 31;

    /* initialize a(2), a(3), a(4) */
    seq_t prev3 = 6;  /* a(n-3) when n=5 */
    seq_t prev2 = 14; /* a(n-2) when n=5 */
    seq_t prev1 = 31; /* a(n-1) when n=5 */
    seq_t an;

    for (idx_t i = 5; i <= n; ++i)
    {
        an = prev1 * 2 + prev2 - prev3; /* a(n) = 2*a(n-1)+a(n-2)-a(n-3) */
        prev3 = prev2;   /* shift for next iteration */
        prev2 = prev1;
        prev1 = an;
    }

    return prev1;
}
