/*
 * A334155  a(n) is the number of length n decorated permutations avoiding the
 *          pattern 001.
 * https://oeis.org/A334155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! + Sum_{j=0}^{n-1} (j+1) * j! (which equals the number
 *            of length‑n decorated permutations avoiding the pattern 001.)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute factorials iteratively and accumulate the sum
       of all factorials up to n.  We keep everything in seq_t
       to allow arbitrary‑precision arithmetic. */
    seq_t fact = (seq_t)1;   /* 0! = 1 */
    seq_t sum  = (seq_t)0;   /* Sum of 1! .. n! (initially 0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        fact = fact * (seq_t)k;  /* k! = (k-1)! * k */
        sum  = sum  + fact;      /* sum += k! */
    }

    /* The sequence value is the sum of all factorials plus
       an extra n! term, as given by the formula.  This adds
       the final n! one more time, matching the OEIS definition. */
    return sum + fact;
}
