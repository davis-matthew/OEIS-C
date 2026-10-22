/*
 * A345097  a(n) is the sum of the two preceding terms if n is odd or n < 8,
 *          and of the two rightmost digits of a(n-1) if n is even, with a(0)
 *          = 0, a(1) = 1.
 * https://oeis.org/A345097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1; for n>=2: if n%2==0 and n>=8: a(n)=sum of the
 *            two rightmost digits of a(n-1); otherwise a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 58, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t prev2 = (seq_t)0; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0 && i >= 8) {          /* even and n>=8 */
            seq_t last_two = prev1 % (seq_t)100;   /* last two decimal digits */
            seq_t d1 = last_two % (seq_t)10;       /* units digit */
            seq_t d2 = last_two / (seq_t)10;       /* tens digit */
            cur = d1 + d2;
        } else {                                /* odd or i<8 */
            cur = prev1 + prev2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
