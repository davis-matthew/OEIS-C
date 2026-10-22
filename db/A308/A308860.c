/*
 * A308860  a(n)/n! is the expected number of left-to-right maxima in the
 *          lexicographical or colexicographical ordering of all the 3-subsets
 *          of [n] under a random permutation of [n], when the 3-subsets hold
 *          the worst order of ranks.
 * https://oeis.org/A308860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)*s(n-1,3) + (2n-1)*s(n-1,2) + (n-2)!
 *            where s(k,ℓ) are the unsigned Stirling numbers of the first
 *            kind. Initial values: a(1)=0, a(2)=0, a(3)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;
    if (n == 3) return (seq_t)6;

    /* Before the loop we have values for n = 2: */
    seq_t fact_prev = (seq_t)1;      /* (2-1)! = 1!  */
    seq_t fact_cur  = (seq_t)2;      /* (3-1)! = 2!  */
    seq_t s2_prev   = (seq_t)1;      /* s(2,2) = 1    */
    seq_t s3_prev   = (seq_t)0;      /* s(2,3) = 0    */
    seq_t a_prev    = (seq_t)0;      /* a(2)   = 0    */

    /* Initialise for n = 3 */
    {
        seq_t i      = (seq_t)3;
        seq_t fact_new = fact_cur * i;                         /* 3!      */
        seq_t s2_curr  = fact_prev + (i - (seq_t)1) * s2_prev; /* s(3,2)=3 */
        seq_t s3_curr  = s2_prev + (i - (seq_t)1) * s3_prev;   /* s(3,3)=1 */

        fact_prev = fact_cur;      /* 2!   */
        fact_cur  = fact_new;      /* 3!   */
        s2_prev   = s2_curr;       /* s(3,2) */
        s3_prev   = s3_curr;       /* s(3,3) */
        a_prev    = (seq_t)6;      /* a(3)   */
    }

    /* Compute a(i) for i = 4 .. n */
    for (idx_t ii = 4; ii <= n; ++ii)
    {
        seq_t i      = (seq_t)ii;
        seq_t fact_new = fact_cur * i;                         /* i!      */
        seq_t s2_curr  = fact_prev + (i - (seq_t)1) * s2_prev; /* s(i,2)  */
        seq_t s3_curr  = s2_prev + (i - (seq_t)1) * s3_prev;   /* s(i,3)  */

        seq_t a_curr = i * a_prev
                      + (i - (seq_t)1) * s3_prev
                      + ((seq_t)2 * i - (seq_t)1) * s2_prev
                      + fact_prev;

        fact_prev = fact_cur;
        fact_cur  = fact_new;
        s2_prev   = s2_curr;
        s3_prev   = s3_curr;
        a_prev    = a_curr;
    }

    return a_prev;
}
