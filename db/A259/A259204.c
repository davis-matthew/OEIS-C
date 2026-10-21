/*
 * A259204  E.g.f.: Sum_{n>=0} x^n * (1 + x^n)^n * exp(4*x^(n+1)) / n!.
 * https://oeis.org/A259204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259204: a(n) = coefficient of x^n in
   A(x) = Σ_{k≥0} x^k (1+x^k)^k exp(4 x^{k+1}) / k!  (EGF)
   a(0)=1, a(1)=5, a(2)=19, a(3)=89, a(4)=377, ... */

/* Helper to compute binomial coefficient for small arguments.
   It works for any idx_t and seq_t types. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = (res * (seq_t)(n - k + i)) / i;
    return res;
}

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4n *= (seq_t)4;

    /* factorials up to n */
    seq_t *fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!fact) return 0;  /* allocation failure */
    fact[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* binom(n, k) for all k=0..n */
    seq_t *bn = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!bn) { free(fact); return 0; }
    bn[0] = 1;
    for (idx_t k = 1; k <= n; ++k)
        bn[k] = (bn[k - 1] * (seq_t)(n - k + 1)) / k;

    seq_t res = pow4n;   /* start with k = 0 term (4^n) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t c_max = (n - k) / (k + 1);
        seq_t pow4c = 1;
        for (idx_t c = 0; c <= c_max; ++c) {
            /* compute j and validate */
            idx_t delta = n - c * (k + 1);
            if (delta < k) {
                if (c < c_max) pow4c *= (seq_t)4;
                continue;
            }
            if (delta % k != 0) {
                if (c < c_max) pow4c *= (seq_t)4;
                continue;
            }
            idx_t j = delta / k - 1;
            if (j > k) {
                if (c < c_max) pow4c *= (seq_t)4;
                continue;
            }

            /* term = binom(n,k) * binom(k,j) * (fact[n-k]/fact[c]) * 4^c */
            seq_t term = bn[k] * binom_small(k, j);
            term *= fact[n - k] / fact[c];
            term *= pow4c;
            res += term;

            if (c < c_max) pow4c *= (seq_t)4;
        }
    }

    free(bn);
    free(fact);
    return res;
}
