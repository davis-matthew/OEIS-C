/*
 * A272125  a(n) = n^3*(2*n^2+1)/3.
 * https://oeis.org/A272125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(2*n^2+1)/3
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = tn * tn;           /* n^2 */
    seq_t prod;

    /* Divide by 3 early to avoid overflow.  3 divides either n or (2*n^2+1). */
    if (tn % 3 == 0)
    {
        /* n is divisible by 3 → factor it out first. */
        seq_t tmp = (tn / 3) * tn;        /* (n/3)*n  */
        seq_t n3  = tmp * tn;            /* (n/3)*n*n = n^3/3  */
        prod = n3 * ((seq_t)2 * n2 + 1); /* (n^3/3)*(2*n^2+1) */
    }
    else
    {
        /* (2*n^2+1) is divisible by 3 → factor it out. */
        seq_t tr  = (seq_t)2 * n2 + 1;   /* 2*n^2+1  */
        seq_t tr3 = tr / 3;              /* (2*n^2+1)/3 */
        prod = tn * tn * tn * tr3;       /* n^3 * ((2*n^2+1)/3) */
    }
    return prod;
}
