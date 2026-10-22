/*
 * A344298  Dirichlet g.f.: Product_{k>=2} (1 + k^(-s))^(9^(k-1)).
 * https://oeis.org/A344298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344298 */
/* Dirichlet g.f.: ∏_{k≥2} (1 + k^(-s))^(9^(k-1)) */

#include "../utils/big_pow.h"

seq_t a(idx_t n);

static seq_t dfs(idx_t n, idx_t min_k)
{
    if (n == 1) return (seq_t)1;   /* empty product */

    /* collect all divisors d ≥ 2 of n, with d ≥ min_k */
    idx_t dlist[128];
    int dcnt = 0;

    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            if (d >= min_k) dlist[dcnt++] = d;
            idx_t d2 = n / d;
            if (d2 != d && d2 >= min_k) dlist[dcnt++] = d2;
        }
    }
    if (n >= min_k) dlist[dcnt++] = n;   /* n itself */

    /* simple insertion sort (dcnt is tiny) */
    for (int i = 1; i < dcnt; ++i) {
        for (int j = i; j > 0 && dlist[j - 1] > dlist[j]; --j)
            { idx_t tmp = dlist[j]; dlist[j] = dlist[j - 1]; dlist[j - 1] = tmp; }
    }

    seq_t total = (seq_t)0;

    for (int idx = 0; idx < dcnt; ++idx) {
        idx_t d = dlist[idx];

        /* maximum exponent of d that divides n */
        idx_t tmp = n;
        int emax = 0;
        while (tmp % d == 0) { tmp /= d; ++emax; }

        /* iterate over possible exponents e = 1 .. emax */
        idx_t power = 1;
        for (int e = 1; e <= emax; ++e) {
            power *= d;  /* d^e */

            /* compute binomial coefficient C(9^(d-1), e) */
            seq_t binom;
            if (e == 1) {
                binom = big_pow((seq_t)9, (seq_t)(d - 1));
            } else {
                seq_t N = big_pow((seq_t)9, (seq_t)(d - 1));

                /* numerator: N * (N-1) * ... * (N-e+1) */
                seq_t numer = (seq_t)1;
                for (int k = 0; k < e; ++k)
                    numer *= (N - (seq_t)k);

                /* denominator: e! */
                seq_t denom = (seq_t)1;
                for (int k = 2; k <= e; ++k)
                    denom *= (seq_t)k;

                binom = numer / denom;
            }

            seq_t rest = dfs(n / power, d + 1);
            total += binom * rest;
        }
    }

    return total;
}

seq_t a(idx_t n)
{
    return dfs(n, 2);
}
