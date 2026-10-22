/*
 * A325997  G.f.: Sum_{n>=0} (n+1) * (x + x^n)^n.
 * https://oeis.org/A325997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum_{k>=0} (k+1) * coeff of x^n in (x + x^k)^k =
 *            \sum_{m>=0} (m+1) * [x^n] x^(m^2) / (1 - x^(m+1))^(m+2) (OEIS
 *            A325997)
 * Range:     u8 n <= 18, u16 n <= 80, u32 n <= 350, u64 n <= 1440,
 *            u128 n <= 5920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, used by idx_t */

/* Generic multiplicative binomial coefficient: C(n,k) for non‑negative integers. */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n - k)          /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* exact division: res * (n-i+1) / i */
        res = res * (seq_t)(n - i + 1);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* outer sum index m ranges from 0 while m^2 <= n */
    for (idx_t m = 0; (m * m) <= n; ++m) {
        idx_t mm = m * m;
        idx_t diff = n - mm;
        idx_t den = m + 1;
        if (diff % den != 0) /* t must be integer */
            continue;
        idx_t t = diff / den;                 /* t corresponds to k in the formula */
        seq_t bin = _binom(m + 1 + t, t);     /* binomial coefficient */
        sum += (seq_t)(m + 1) * bin;          /* (m+1) * binom(...) */
    }
    return sum;
}
