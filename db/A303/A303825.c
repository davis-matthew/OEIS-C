/*
 * A303825  Number of ways of writing n as a sum of powers of 7, each power
 *          being used at most seven times.
 * https://oeis.org/A303825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for k>0: a(7*k)=a(k)+a(k-1); for r=1..6: a(7*k+r)=a(k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) */

    if (n % 7 == 0) {      /* n = 7*k */
        idx_t k = n / 7;
        return a(k) + a(k - 1);   /* a(7*k) = a(k)+a(k-1) */
    } else {              /* n = 7*k + r, r=1..6 */
        return a(n / 7);       /* a(7*k+r) = a(k) */
    }
}
