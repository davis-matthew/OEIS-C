/*
 * A361703  Constant term in the expansion of (1 + w + x + y + z +
 *          1/(w*x*y*z))^n.
 * https://oeis.org/A361703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/5⌋} (5k)!/(k!^5) * binomial(n,5k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans   = (seq_t)1;      /* term for k = 0 */
    seq_t bin   = (seq_t)1;      /* C(n,0)      */
    seq_t term  = (seq_t)1;      /* (5·0)!/(0!^5) */
    idx_t maxk  = n / 5;         /* floor(n/5) */

    for (idx_t k = 1; k <= maxk; ++k) {
        /* update binomial C(n,5k) from C(n,5(k-1)) */
        idx_t base = 5 * (k - 1);
        for (idx_t i = 1; i <= 5; ++i) {
            idx_t index = base + i;                                  /* m+1 */
            bin = (bin * ((seq_t)n - (seq_t)index + 1)) / (seq_t)index;
        }

        /* update T(k) = (5k)!/(k!^5) from T(k-1) */
        seq_t denom = (seq_t)k;
        for (idx_t i = 1; i <= 5; ++i) {
            seq_t mult = (seq_t)(5 * (k - 1) + i);  /* 5(k-1)+i */
            term = (term * mult) / denom;
        }

        ans += bin * term;
    }

    return ans;
}
