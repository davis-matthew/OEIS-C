/*
 * A398922  Number of distinct acute lattice trapezia of area n/2 with
 *          horizontal base.
 * https://oeis.org/A398922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute f(m) as defined above.  The helper is defined outside 'a' because
 * C forbids static inline function definitions inside a function body. */
static inline seq_t f_of_m(seq_t m)
{
    if (m < 4) 
        return 0;

    seq_t K = m - 4;          /* K = m-4 >= 0 */
    seq_t t = K >> 1;         /* floor(K/2) */
    return (t + 1) * (K + 1 - t);
}
/*  OEIS A398922
 *  Number of distinct acute lattice trapezia of area n/2 with horizontal base.
 *  Equivalent combinatorial formula:
 *     a(n) = Σ_{d|n} f(n/d)
 *  where
 *     f(m) = 0                     if m < 4
 *     f(m) = (t+1) * (K+1 - t)     otherwise,
 *     with K = m-4   and   t = floor(K/2).
 *  The function below implements this formula.  It enumerates all divisors
 *  of n in O(√n) time, which is well below the required limits for n≤10000.
 */

seq_t a(idx_t n)
{
    /* Iterate over all divisors d of n.  For each divisor we compute
       m = n/d and add f(m) to the sum. */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t m1 = n / d;            /* divisor pair (d, n/d) */
            sum += f_of_m(m1);
            if (d * d != n) {            /* add the complementary divisor */
                seq_t m2 = d;
                sum += f_of_m(m2);
            }
        }
    }
    return sum;
}
