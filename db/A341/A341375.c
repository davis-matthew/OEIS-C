/*
 * A341375  G.f. A(x) = Sum_{n>=0} a(n)*x^n satisfies: [Sum_{n>=0} x^n/(1 -
 *          x^(n+1))]^4 = Sum_{n>=0} a(n)*x^n/(1 - x^(n+1))^4.
 * https://oeis.org/A341375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A341375  – Computation of the sequence
 *
 *  Formula:  a(0)=1, and for n>0
 *
 *  a(n) = S(n) – Σ_{d|n+1, d<n+1} a(d-1) * C((n+1)/d + 2, 3)
 *
 *  where S(n) is the n‑th coefficient of ( Σ_{k≥0} τ(k+1) x^k )^4,
 *  and τ(m) is the number of divisors of m.
 *
 *  The implementation keeps a cache of already computed terms and
 *  extends it incrementally when higher indices are requested.
 */

#include <stdlib.h>

static inline seq_t binom3(seq_t k)
{
    seq_t num = k;
    num *= k + 1;
    num *= k + 2;
    num /= 6;          /* divisible by 6 */
    return num;
}

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *a_vals  = NULL;   /* a[i]              */
    static seq_t *g_vals  = NULL;   /* τ(i+1)            */
    static seq_t *g2_vals = NULL;   /* convolution 2     */
    static seq_t *g3_vals = NULL;   /* convolution 3     */
    static seq_t *g4_vals = NULL;   /* convolution 4     */
    static size_t cache_len = 0;    /* number of cached terms */

    if ((size_t)n >= cache_len) {
        size_t new_len  = (size_t)n + 1;

        /* extend the arrays */
        a_vals  = (seq_t*)realloc(a_vals,  new_len * sizeof(seq_t));
        g_vals  = (seq_t*)realloc(g_vals,  new_len * sizeof(seq_t));
        g2_vals = (seq_t*)realloc(g2_vals, new_len * sizeof(seq_t));
        g3_vals = (seq_t*)realloc(g3_vals, new_len * sizeof(seq_t));
        g4_vals = (seq_t*)realloc(g4_vals, new_len * sizeof(seq_t));

        /* initialise new slots to zero */
        for (size_t i = cache_len; i < new_len; ++i) {
            a_vals[i]  = 0;
            g_vals[i]  = 0;
            g2_vals[i] = 0;
            g3_vals[i] = 0;
            g4_vals[i] = 0;
        }

        /* compute all new terms incrementally */
        for (size_t i = cache_len; i < new_len; ++i) {

            /* 1. g[i] = τ(i+1) */
            size_t m      = i + 1;
            size_t count  = 0;
            for (size_t d = 1; d*d <= m; ++d) {
                if (m % d == 0) {
                    ++count;
                    if (d*d != m) ++count;
                }
            }
            g_vals[i] = (seq_t)count;

            /* 2. g2[i] = Σ_{k=0..i} g[k]*g[i-k] */
            seq_t val2 = 0;
            for (size_t k = 0; k <= i; ++k) {
                val2 += g_vals[k] * g_vals[i - k];
            }
            g2_vals[i] = val2;

            /* 3. g3[i] = Σ_{k=0..i} g2[k]*g[i-k] */
            seq_t val3 = 0;
            for (size_t k = 0; k <= i; ++k) {
                val3 += g2_vals[k] * g_vals[i - k];
            }
            g3_vals[i] = val3;

            /* 4. g4[i] = Σ_{k=0..i} g3[k]*g[i-k] */
            seq_t val4 = 0;
            for (size_t k = 0; k <= i; ++k) {
                val4 += g3_vals[k] * g_vals[i - k];
            }
            g4_vals[i] = val4;

            /* 5. compute a[i] from the recurrence */
            seq_t sum_other = 0;
            for (size_t d = 1; d*d <= m; ++d) {
                if (m % d == 0) {
                    /* divisor d < m */
                    if (d < m) {
                        size_t tdiv = m / d;
                        sum_other += a_vals[d - 1] * binom3((seq_t)tdiv);
                    }
                    /* complementary divisor */
                    size_t d2 = m / d;
                    if (d2 != d && d2 < m) {
                        size_t tdiv = m / d2;   /* which equals d */
                        sum_other += a_vals[d2 - 1] * binom3((seq_t)tdiv);
                    }
                }
            }

            a_vals[i] = g4_vals[i] - sum_other;
        }

        cache_len = new_len;
    }

    return a_vals[(size_t)n];
}
