/*
 * A260114  Expansion of f(x)^4 * phi(-x^3) / phi(-x) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A260114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in q^(-1/6)*eta(q^2)^13*eta(q^3)^2 *
 *            /(eta(q)^6*eta(q^4)^4*eta(q^6)) * The result is the OEIS
 *            sequence A260114. The implementation * constructs the power
 *            series of the eta‑quotient directly
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* small binomial coefficient n choose k for n <= 13 */
static long long binom_small(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    long long res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    /* work only on the needed range */
    size_t sz = (size_t)n + 1;
    long long *a = (long long *)calloc(sz, sizeof(long long));
    long long *b = (long long *)calloc(sz, sizeof(long long));
    if (!a || !b) { /* guard against allocation failure */
        return (seq_t)0;
    }
    a[0] = 1;                     /* start with 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        int e = -6;
        if ((k & 1ULL) == 0)  e += 13;         /* k even */
        if (k % 3ULL == 0)    e += 2;          /* multiple of 3 */
        if (k % 4ULL == 0)    e -= 4;          /* multiple of 4 */
        if (k % 6ULL == 0)    e -= 1;          /* multiple of 6 */

        if (e == 0) continue;                 /* nothing to do */

        if (e > 0)
        {
            /* multiply by (1 - q^k)^e  ,  e <= 13 */
            for (size_t npos = 0; npos < sz; ++npos)
            {
                __int128 sum = 0;
                size_t maxr = npos / k;
                if (maxr > (size_t)e) maxr = (size_t)e;
                for (size_t r = 0; r <= maxr; ++r)
                {
                    long long term = a[npos - r * k];
                    long long bin = binom_small(e, (int)r);
                    if (r & 1U)
                        sum -= (__int128)term * bin;
                    else
                        sum += (__int128)term * bin;
                }
                b[npos] = (long long)sum;
            }
        }
        else /* e < 0 : multiply by 1/(1 - q^k)^{-e} */
        {
            int m = -e;                      /* 1 <= m <= 6 */
            size_t max_t = n / k;
            unsigned long long *comb = (unsigned long long *)
                                        malloc((max_t + 1) * sizeof(unsigned long long));
            if (!comb) { free(a); free(b); return (seq_t)0; }
            comb[0] = 1;
            for (size_t t = 1; t <= max_t; ++t)
            {
                __int128 val = (__int128)comb[t - 1] * (t + m - 1);
                val /= t;
                comb[t] = (unsigned long long)val;
            }
            for (size_t npos = 0; npos < sz; ++npos)
            {
                __int128 sum = 0;
                size_t max_tn = npos / k;
                if (max_tn > max_t) max_tn = max_t;
                for (size_t t = 0; t <= max_tn; ++t)
                    sum += (__int128)a[npos - t * k] * comb[t];
                b[npos] = (long long)sum;
            }
            free(comb);
        }

        /* swap arrays for next iteration */
        long long *tmp = a;
        a = b;
        b = tmp;
    }

    seq_t result = (seq_t)a[n];
    free(a);
    free(b);
    return result;
}
