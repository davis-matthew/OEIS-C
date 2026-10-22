/*
 * A374980  Number of multiset permutations of {1, 1, 2, 2, ..., n, n} with no
 *          fixed pair (j,j).
 * https://oeis.org/A374980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=0..n} (-1)^{\,n-r} * C(n,r) * (2r)! / 2^r
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the permutations of {1,1,2,2,…,n,n} in which the two copies   */
/* of each j are not both placed at positions (2j-1,2j).                       */
seq_t a(idx_t n)
{
    seq_t result_pos = 0;   /* sum of terms with positive sign   */
    seq_t result_neg = 0;   /* sum of terms with negative sign  */
    seq_t binom      = 1;   /* binomial(n,0)                      */
    seq_t term       = 1;   /* (2*0)!/2^0 = 1                     */

    for (idx_t r = 0; r <= n; ++r)
    {
        /* sign = +1 if (n-r) is even, else -1      */
        unsigned int sign_positive = ((n - r) & 1) ? 0 : 1;
        seq_t prod = binom * term;

        if (sign_positive)
            result_pos += prod;
        else
            result_neg += prod;

        if (r == n)  /* no further updates needed after last term   */
            break;

        /* Update binomial coefficient for next r (r+1)    */
        binom = (binom * (seq_t)(n - r)) / (seq_t)(r + 1);

        /* Update term from (2r)!/2^r to (2(r+1))!/2^(r+1) */
        /*   (2(r+1))! = (2r)! * (2r+1) * (2r+2)           */
        /*   2^(r+1)   = 2^r * 2                         */
        term = (term * (seq_t)(2 * r + 1) * (seq_t)(2 * r + 2)) / 2;
    }

    /* The final result is guaranteed positive */
    return result_pos - result_neg;
}
