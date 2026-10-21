/*
 * A277178  a(n) = Sum_{k=0..n} k*binomial(2*k,k)/2.
 * https://oeis.org/A277178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(n)=a(n-1)+(2*n-1)*C(2*n-2,n-1) for n>=1 (here C
 *            denotes the binomial coefficient)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;          /* running sum a(n)              */
    seq_t binom  = 1;          /* C(0,0)                         */

    for (idx_t i = 1; i <= n; ++i) {
        /* add the term for current i */
        seq_t term = (seq_t)(2 * i - 1) * binom;
        result += term;

        /* if we have reached the last term, no need to update binom */
        if (i == n)
            break;

        /* update binom to C(2*i, i) for the next iteration:
           binom_new = binom * (2*i-1)/i * (2*i)/i   (exact divisions) */
        binom = (binom * (seq_t)(2 * i - 1)) / i;
        binom = (binom * (seq_t)(2 * i    )) / i;
    }

    return result;
}
