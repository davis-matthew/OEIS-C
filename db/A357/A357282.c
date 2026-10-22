/*
 * A357282  a(n) = number of subsets S of {1,2,...,n} having more than 1
 *          element such that (difference between least two elements of S) =
 *          difference between greatest two elements of S.
 * https://oeis.org/A357282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2) + Σ_{d=1}^{n-1} (n-d) * f(d) where f(d) =
 *            (2^{d-1}-1)/3 if d is odd = (2^{d-1}+1)/3 if d is even This
 *            implements the sum over all pairs (a,b) with b-a=d
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                      /* a(0)=a(1)=0 */

    seq_t result = (seq_t)n * ((seq_t)n - 1) / 2;  /* binomial(n,2) */

    seq_t pow2 = 1;                    /* 2^{d-1} for current d */
    for (idx_t diff = 1; diff < n; ++diff)
    {
        seq_t term;
        if (diff & 1)                  /* odd diff */
            term = (pow2 - 1) / 3;
        else                           /* even diff */
            term = (pow2 + 1) / 3;

        result += term * ((seq_t)(n - diff));
        pow2 = pow2 * 2;               /* advance 2^{d-1} -> 2^{d} */
    }
    return result;
}
