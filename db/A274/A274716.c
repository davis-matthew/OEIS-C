/*
 * A274716  a(2*n+1) = a(2*floor(n/2)+1) + n, a(2*n) = a(n), for n>=1 with
 *          a(1)=0.
 * https://oeis.org/A274716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = a(n); a(2*n+1) = a(2*floor(n/2)+1) + n; a(1) = 0.
 *            (Closed form: for odd t, a(t) = t-1 - popcount((t-1)/2).)
 * Range:     u8 n <= 128, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Strip trailing zeros; the value is unchanged for powers of two. */
    while ((n & 1) == 0)
        n >>= 1;

    if (n == 1)
        return 0;          /* a(1) = 0 */

    /* k = (t-1)/2 where t is the odd part of n. */
    seq_t k = ((seq_t)(n - 1)) >> 1;

    /* popcount of k */
    seq_t cnt = 0;
    seq_t tmp = k;
    while (tmp) {
        cnt += (tmp & 1);
        tmp >>= 1;
    }

    return 2 * k - cnt;    /* a(t) = 2k - popcount(k) */
}
