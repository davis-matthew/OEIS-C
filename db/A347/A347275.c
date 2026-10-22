/*
 * A347275  a(n) is the number of nonnegative ordered pairs (a,b) satisfying
 *          (a+b <= n) and (a*b <= n).
 * https://oeis.org/A347275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1) + Σ_{a = 1}^{n} (min(n - a, ⌊n / a⌋) + 1) i.e.
 *            the number of nonnegative ordered pairs (a, b) with a + b ≤ n
 *            and a · b ≤ n
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case: n = 0 gives the single pair (0, 0). */
    seq_t res = ((seq_t)n * 2) + 1;   /* 2n + 1  */

    for (idx_t a = 1; a <= n; ++a) {
        seq_t t   = (seq_t)(n / a);            /* floor(n / a)   */
        seq_t rem = (seq_t)(n - a);            /* n - a          */
        seq_t minv = (t < rem) ? t : rem;      /* min(n-a, floor(n/a)) */
        res += minv;
    }

    return res;
}
