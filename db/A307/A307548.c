/*
 * A307548  Expansion of Product_{k>=1} (1 - (x/(1+x))^k).
 * https://oeis.org/A307548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - (x/(1+x))^k). Using
 *            Euler's pentagonal theorem: ∏_{k≥1} (1 - q^k) = Σ_{t∈ℤ} (-1)^t
 *            q^{g(t)} with g(t)=t(3t-1)/2. Substituting q=x/(1+x) and
 *            expanding (x/(1+x))^m gives a(n) = Σ_{t∈ℤ, g(t)≤n}
 *            (-1)^{t+n-g(t)} * C(n-1, n-g(t)) (C denotes binomial
 *            coefficient...
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Simple multiplicative binomial coefficient: C(n,k) */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;   /* a(0) = 1 */

    seq_t sum = (seq_t)0;
    /* iterate over pentagonal indices t=1,2,… while g(t)≤n */
    for (idx_t t = (idx_t)1; ; ++t) {
        /* g_pos = t(3t-1)/2,  g_neg = t(3t+1)/2 */
        idx_t gpos = t * ((idx_t)3 * t - (idx_t)1) / (idx_t)2;
        if (gpos > n)
            break;  /* further g_pos will only increase */

        idx_t rpos = n - gpos;
        seq_t bin = _binom(n - (idx_t)1, rpos);
        if ( ((t + n - gpos) & (idx_t)1) == 0 )
            sum = sum + bin;
        else
            sum = sum - bin;

        idx_t gneg = t * ((idx_t)3 * t + (idx_t)1) / (idx_t)2;
        if (gneg <= n) {
            idx_t rneg = n - gneg;
            seq_t bin2 = _binom(n - (idx_t)1, rneg);
            if ( ((t + n - gneg) & (idx_t)1) == 0 )
                sum = sum + bin2;
            else
                sum = sum - bin2;
        }
    }

    return sum;
}
