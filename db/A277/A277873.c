/*
 * A277873  Number of ways of writing n as a sum of powers of 5, each power
 *          being used at most five times.
 * https://oeis.org/A277873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of powers of 5, each
 *            power being used at most five times. Recurrence: a(0)=1; for
 *            k>0, a(5k)=a(k)+a(k-1) and a(5k+r)=a(k) (r=1,2,3,4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t rec(idx_t n)
{
    /* Base case – n < 5: only 1's may be used. */
    if (n < 5)
        return (seq_t)1;

    /* Reduce the problem by a factor of 5. */
    idx_t k = n / 5;
    idx_t r = n % 5;

    if (r == 0)                /* n = 5*k */
        return rec(k) + rec(k - 1);
    else                        /* n = 5*k + r, 1 ≤ r ≤ 4 */
        return rec(k);
}

seq_t a(idx_t n)
{
    return rec(n);
}
