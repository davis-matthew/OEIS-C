/*
 * A281005  Numbers n having at least one odd divisor greater than sqrt(2*n).
 * https://oeis.org/A281005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer n for which the odd part of n exceeds
 *            2^{v2(n)+1}
 * Range:     u8 n <= 198, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, if n = 2^e * m with m odd, then m > 2^{e+1}. */

seq_t a(idx_t n)
{
    idx_t cur = 1;          /* candidate integer */
    idx_t count = 0;        /* number of qualifying integers found so far */

    while (1)
    {
        /* Separate the power of two factor and the odd part. */
        idx_t tmp = cur;
        idx_t e = 0;
        while ((tmp & 1) == 0)        /* while even */
        {
            tmp >>= 1;               /* remove a factor of 2 */
            ++e;
        }
        seq_t odd = tmp;              /* odd part m */

        /* Compute 2^{e+1} as a seq_t value. */
        seq_t threshold = ((seq_t)1) << (e + 1);

        if (odd > threshold)         /* check the condition */
        {
            ++count;
            if (count == n)           /* found the n-th term */
                return (seq_t)cur;
        }

        ++cur;                        /* next candidate */
    }
}
