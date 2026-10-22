/*
 * A345245  G.f. A(x) satisfies: A(x) = x + x^2 * exp(4 * Sum_{k>=1}
 *          (-1)^(k+1) * A(x^k) / k).
 * https://oeis.org/A345245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:  a(1)=1, a(2)=1
    a(n+2) = (4/n) * Σ_{k=1..n} ( Σ_{d|k} (-1)^{k/d+1} * d * a(d) ) * a(n-k+2)   */
seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;   /* array of sequence values, 1‑based */
    static seq_t *inner  = NULL;   /* inner sums Σ_{d|k} (-1)^{k/d+1} d a(d) */
    static size_t cap = 0;         /* capacity of the arrays */

    if (n < 1)
        return 0;                 /* not used in the tests */

    /* allocate initial space for indices 0,1,2 */
    if (a_vals == NULL) {
        cap = 3;
        a_vals = (seq_t *)malloc(cap * sizeof(seq_t));
        inner   = (seq_t *)malloc(cap * sizeof(seq_t));

        a_vals[1] = (seq_t)1;
        a_vals[2] = (seq_t)1;
        inner[1]  = (seq_t)1;      /* inner[1] = 1 * a(1) */
    }

    /* extend storage if needed and compute missing terms */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;        /* use 0…n inclusive */
        a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        inner   = (seq_t *)realloc(inner,   new_cap * sizeof(seq_t));
        for (size_t i = cap; i < new_cap; ++i) {
            a_vals[i] = (seq_t)0;
            inner[i]  = (seq_t)0;
        }

        for (size_t m = cap; m <= (size_t)n; ++m) {
            if (m < 3) continue;            /* already set */

            /* compute inner sum for k = m-2 if not done yet */
            size_t k = m - 2;
            if (inner[k] == (seq_t)0) {
                seq_t pos = 0;
                seq_t neg = 0;
                for (size_t d = 1; d * d <= k; ++d) {
                    if (k % d == 0) {
                        size_t t1 = d;
                        size_t t2 = k / d;

                        /* process divisor t1 */
                        size_t exp1 = (k / t1) + 1;
                        seq_t prod1 = (seq_t)t1 * a_vals[t1];
                        if ((exp1 & 1ULL) == 0)   /* even => positive */
                            pos += prod1;
                        else
                            neg += prod1;

                        /* process divisor t2 if distinct */
                        if (t1 != t2) {
                            size_t exp2 = (k / t2) + 1;
                            seq_t prod2 = (seq_t)t2 * a_vals[t2];
                            if ((exp2 & 1ULL) == 0)
                                pos += prod2;
                            else
                                neg += prod2;
                        }
                    }
                }
                inner[k] = pos - neg;           /* pos ≥ neg for all k */
            }

            /* compute a[m] using the recurrence */
            seq_t acc = 0;
            for (size_t kk = 1; kk <= k; ++kk) {
                acc += inner[kk] * a_vals[m - kk];
            }
            seq_t num = acc * (seq_t)4;
            seq_t den = (seq_t)(m - 2);
            a_vals[m] = num / den;
        }
        cap = new_cap;
    }

    return a_vals[n];
}
