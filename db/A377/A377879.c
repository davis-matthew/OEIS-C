/*
 * A377879  Deficiency of squares: a(n) = 2n^2 - sigma(n^2).
 * https://oeis.org/A377879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*n^2 - sigma(n^2)
 * Range:     i8 n <= 12, i16 n <= 167, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 78 terms through n = 9742 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t sigma_square_of(idx_t n)
{
    seq_t sigma = 1;
    idx_t remaining = n;

    for (idx_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            /* count exponent e of prime p in n */
            idx_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            /* term = 1 + p + p^2 + … + p^(2e) */
            seq_t term = 1;
            seq_t pow = 1;
            for (idx_t i = 0; i < 2 * e; ++i) {
                pow *= (seq_t)p;
                term += pow;
            }
            sigma *= term;
        }
    }

    /* any remaining prime factor with exponent 1 */
    if (remaining > 1) {
        seq_t term = 1 + (seq_t)remaining + (seq_t)remaining * (seq_t)remaining;
        sigma *= term;
    }

    return sigma;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset starts at 1, but safeguard */

    seq_t n_t = (seq_t)n;
    seq_t n2  = n_t * n_t;
    seq_t sigma_sq = sigma_square_of(n);
    return 2 * n2 - sigma_sq;
}
