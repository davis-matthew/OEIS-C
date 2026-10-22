/*
 * A328348  Let S be any integer in the range 3 <= S <= 17. Sequence has the
 *          property that a(n)*S is the sum of all positive integers whose
 *          decimal expansion has <= n digits and contains at most two
 *          distinct nonzero digits p and q such that p+q=S.
 * https://oeis.org/A328348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1, and for n>1 * a(n) = 21*a(n-1) - 20*a(n-2) +
 *            2^(n-1) (OEIS A328348)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)0;          /* a(n-2) */
    seq_t a1 = (seq_t)1;          /* a(n-1) */
    seq_t pow2 = (seq_t)2;        /* 2^(n-1), initially 2^1 for n==2 */

    /* Compute a(2) ... a(n) iteratively */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 21*a1 - 20*a0 + pow2  */
        seq_t ai = 21 * a1;
        ai -= 20 * a0;
        ai += pow2;

        /* Prepare for next iteration */
        a0 = a1;
        a1 = ai;
        pow2 *= 2;                /* advance 2^(i-1) -> 2^i */
    }

    return a1;   /* a(n) */
}
