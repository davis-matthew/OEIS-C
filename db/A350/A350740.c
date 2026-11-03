/*
 * A350740  Number of integer points (x, y, z, w) at distance <= 1/2 from
 *          3-sphere of radius n.
 * https://oeis.org/A350740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * (E(n² + n) - E(n² - n)), for n > 0 a(0) = 1 where
 *            E(m) = Σ_{d=1}^{m} d * ⌊m/d⌋ - 4 * Σ_{d=1}^{⌊m/4⌋} d *
 *            ⌊(m/4)/d⌋. This counts the integer lattice points whose distance
 *            to the 3‑sphere of radius n is ≤ ½. The summations are evaluated
 *            in O(√m) time by grouping equal quotients. All arit...
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 601, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9470 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, needed only for comments */

/* ---------------------------------------------------------
   Compute Σ_{d=1}^{M} d * ⌊M/d⌋ in O(√M) time.             */
static seq_t sum_d_floor(idx_t M)
{
    seq_t total = 0;
    for (idx_t l = 1; l <= M; ) {
        /* For all d in [l, r] the quotient t = ⌊M/d⌋ is the same. */
        idx_t t = M / l;
        idx_t r = M / t;          /* r = largest d with ⌊M/d⌋ = t */

        seq_t cnt = (seq_t)(r - l + 1);
        seq_t sum_lr = ((seq_t)l + (seq_t)r) * cnt / 2;

        total += sum_lr * (seq_t)t;
        l = r + 1;
    }
    return total;
}

/* ---------------------------------------------------------
   Evaluate the function E(m) as defined in the formula.     */
static seq_t eval_E(idx_t M)
{
    /* Basic term Σ d * ⌊M/d⌋. */
    seq_t s1 = sum_d_floor(M);

    /* Subtract contributions of divisors divisible by 4. */
    idx_t quarter = M / 4;
    seq_t s2 = sum_d_floor(quarter);

    return s1 - (seq_t)4 * s2;
}

/* Forward declarations of helper functions making only the allowed
   arithmetic and control constructs. */
static seq_t sum_d_floor(idx_t M);
static seq_t eval_E(idx_t M);

/* Main entry function required by the spec. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute the two bounds of the interval. */
    idx_t excl = n * n;          /* n² */
    idx_t m1 = excl + n;         /* n² + n   */
    idx_t m2 = excl - n;         /* n² - n   */

    /* Evaluate E for both bounds and apply the formula. */
    seq_t e1 = eval_E(m1);
    seq_t e2 = eval_E(m2);

    return (seq_t)8 * (e1 - e2);
}
