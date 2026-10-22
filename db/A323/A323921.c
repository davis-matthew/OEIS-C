/*
 * A323921  a(n) = (4^(valuation(n, 4) + 1) - 1) / 3.
 * https://oeis.org/A323921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (4^(valuation(n,4)+1)-1)/3
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute v4(n), the exponent of 4 dividing n */
    idx_t t = n;
    unsigned int e = 0;
    while ((t & 3) == 0) {   /* n is divisible by 4 */
        ++e;
        t >>= 2;             /* divide n by 4 for the next check */
    }

    /* Compute (4^(e+1)-1)/3 using the recurrence:
       A_0 = 1
       A_{k+1} = 4*A_k + 1
       For e iterations we get the required value. */
    seq_t ans = 1;
    for (unsigned int i = 0; i < e; ++i)
        ans = ans * 4 + 1;

    return ans;
}
