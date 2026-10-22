/*
 * A330795  Evaluation of the polynomials given by the Riordan square of the
 *          Fibonacci sequence with a(0) = 1 (A193737) at 1/2 and normalized
 *          with 2^n.
 * https://oeis.org/A330795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) (n>=3) Initial values: a(0)=1,
 *            a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)3;
    if (n == 2u)
        return (seq_t)9;

    seq_t prev2 = (seq_t)3;   /* a(n-2) for current iteration */
    seq_t prev1 = (seq_t)9;   /* a(n-1) for current iteration */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = prev1 * (seq_t)3 + prev2 * (seq_t)4;  /* a(n) */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
