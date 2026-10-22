/*
 * A351757  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 3*x)) / (1 - 3*x)^2.
 * https://oeis.org/A351757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1; * a(n) = Σ_{k=1..n} binomial(n,k-1) * 3^(k-1) *
 *            a(n-k) for n ≥ 1 * = Σ_{i=0..n-1} binomial(n,i) * 3^i * a(n-1-i)
 *            * Recurrence is implemented using memoisation
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache  = NULL;
    static size_t a_cap    = 0; /* allocated capacity for a_cache */
    static seq_t *pow3    = NULL;
    static size_t pow_cap  = 0; /* allocated capacity for pow3 */
    static idx_t  max_computed = (idx_t)-1; /* last index computed */

    /* initial allocation */
    if (a_cache == NULL) {
        a_cap = 8;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        pow_cap = 8;
        pow3   = (seq_t*)malloc(pow_cap * sizeof(seq_t));
        if (!pow3) exit(EXIT_FAILURE);
        a_cache[0] = 1;
        pow3[0]    = 1;
        max_computed = 0;
    }

    /* extend cached values up to n */
    if (n > max_computed) {
        /* ensure sufficient storage */
        size_t need = (size_t)(n + 1);
        if (a_cap < need) {
            size_t new_cap = a_cap * 2;
            if (new_cap < need) new_cap = need + 8;
            seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            a_cache = tmp;
            a_cap = new_cap;
        }
        if (pow_cap < need) {
            size_t new_cap = pow_cap * 2;
            if (new_cap < need) new_cap = need + 8;
            seq_t *tmp = (seq_t*)realloc(pow3, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            pow3 = tmp;
            pow_cap = new_cap;
        }

        /* compute new terms */
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            seq_t sum   = 0;
            seq_t binom = 1;          /* binomial(n,0) */
            for (idx_t i = 0; i < m; ++i) {
                /* term = binom * 3^i * a(m-1-i) */
                seq_t term = binom * pow3[i];
                term = term * a_cache[m - 1 - i];
                sum = sum + term;

                /* update binom for next i: binom = binom * (m - i) / (i + 1) */
                seq_t num = m - i;
                seq_t den = i + 1;
                binom = (binom * num) / den;
            }
            a_cache[m] = sum;

            /* pre‑compute 3^m for later iterations */
            pow3[m] = pow3[m - 1] * 3;
        }
        max_computed = n;
    }

    return a_cache[n];
}
