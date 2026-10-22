/*
 * A342352  Expansion of e.g.f. (exp(x)-1)*(exp(x) - x^2/2 - x - 1).
 * https://oeis.org/A342352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - ( 2 + n + n*(n-1)/2 ) with a(n)=0 for n<3 * This
 *            matches the OEIS definition a(n) = 2^n -
 *            [binom(n,0)+binom(n,1)+binom(n,2)+binom(n,n)]. * The function
 *            returns 0 for n = 0,1,2; for n ≥ 3 it evaluates the formula
 *            exactly. * Complexity: O(n) multiplications per call, well below
 *            the 2‑s...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the small indices directly */
    if (n < 3)
        return 0;

    /* compute 2^n in seq_t, respecting modulo behavior of the target type */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* binomial sum: C(n,0)+C(n,1)+C(n,2)+C(n,n) = 2 + n + n*(n-1)/2 */
    seq_t bin = 2 + (seq_t)n + (seq_t)n * (n - 1) / 2;

    return pow2 - bin;
}
