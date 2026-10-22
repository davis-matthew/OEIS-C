/*
 * A328259  a(n) = n * sigma_2(n).
 * https://oeis.org/A328259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * sigma_2(n) (sigma_2: sum of squares of divisors)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 618, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    seq_t sigma2 = 1;                          /* product of divisor‑square sums */
    idx_t m = n;                               /* remainder to factorise */

    /* factorise n, processing each prime power separately */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;                        /* exponent of p in n */
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            seq_t p2  = (seq_t)p * (seq_t)p;    /* p^2 */
            seq_t powp = 1;                     /* p^(2*(e+1)) */
            for (idx_t i = 0; i < (idx_t)2 * (e + 1); ++i) {
                powp *= (seq_t)p;
            }
            seq_t numer = powp - 1;             /* p^(2*(e+1)) - 1 */
            seq_t denom = p2 - 1;               /* p^2 - 1 */
            seq_t sum   = numer / denom;        /* exact division */
            sigma2 *= sum;
        }
    }

    /* any remaining prime factor p^1 */
    if (m > 1) {
        seq_t p   = (seq_t)m;                   /* remaining prime */
        seq_t p2  = p * p;                      /* p^2 */
        seq_t powp = 1;                         /* p^4  (2*(1+1)=4) */
        for (int i = 0; i < 4; ++i) {
            powp *= p;
        }
        seq_t numer = powp - 1;                 /* p^4 - 1 */
        seq_t denom = p2 - 1;                   /* p^2 - 1 */
        seq_t sum   = numer / denom;
        sigma2 *= sum;
    }

    return (seq_t)n * sigma2;                   /* a(n) = n * sigma_2(n) */
}
