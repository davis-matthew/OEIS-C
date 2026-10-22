/*
 * A372177  E.g.f. A(x) satisfies A(x) = exp( x * A(x)^(1/2) * (1 +
 *          A(x)^(1/2)) ).
 * https://oeis.org/A372177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372177:
   a(0) = 1
   The exponential generating function satisfies
   A(x) = exp( x * sqrt(A(x)) * (1 + sqrt(A(x))) ).
   This implementation uses the recurrence
      a_n = ∑_{k=1}^{n} C(n-1, k-1) * k * (s_{k-1} + a_{k-1}) * a_{n-k},
   where s_0 = 1 and a_n = (s^2)_n.
   The intermediate values s_n are obtained from
      s_n = (a_n - ∑_{i=1}^{n-1} C(n, i) * s_i * s_{n-i}) / 2.
   All calculations are performed with the generic integer type seq_t.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static tables for memoisation */
    static seq_t *s = NULL;   /* sequence s_n */
    static seq_t *a_seq = NULL; /* sequence a_n */
    static size_t cap = 0;     /* allocated capacity */
    static idx_t computed = 0; /* highest n already computed */

    /* Ensure we have space for indices up to n */
    if (s == NULL) {
        cap = n + 1;
        s = (seq_t*)malloc(cap * sizeof(seq_t));
        a_seq = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!s || !a_seq) abort();          /* memory failure */
        for (size_t i = 0; i < cap; ++i) {
            s[i] = 0;
            a_seq[i] = 0;
        }
        /* initialise base case */
        s[0] = 1;
        a_seq[0] = 1;
        computed = 0;
    } else if ((size_t)(n + 1) > cap) {
        size_t new_cap = n + 1;
        s = (seq_t*)realloc(s, new_cap * sizeof(seq_t));
        a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!s || !a_seq) abort();          /* memory failure */
        for (size_t i = cap; i < new_cap; ++i) {
            s[i] = 0;
            a_seq[i] = 0;
        }
        cap = new_cap;
    }

    /* compute values up to n */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* ----- compute a_m using the recurrence ----- */
        seq_t sum_a = 0;
        seq_t binom_row = 1;   /* C(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t f_k = (seq_t)k * (s[k - 1] + a_seq[k - 1]); /* f_k = k*(s_{k-1}+a_{k-1}) */
            seq_t term = binom_row * f_k * a_seq[m - k];
            sum_a += term;
            if (k < m) {
                /* update binom_row to C(m-1, k) */
                binom_row = binom_row * (seq_t)(m - k) / (seq_t)k;
            }
        }
        a_seq[m] = sum_a;

        /* ----- compute s_m using the definition of convolution ----- */
        seq_t sum_s2 = 0;
        seq_t binom_val = 1;    /* will become C(m,1) in first iteration */
        for (idx_t i = 1; i <= m - 1; ++i) {
            if (i == 1) {
                binom_val = (seq_t)m;
            } else {
                binom_val = binom_val * (seq_t)(m - i + 1) / (seq_t)i;
            }
            sum_s2 += binom_val * s[i] * s[m - i];
        }
        s[m] = (a_seq[m] - sum_s2) / 2;
    }
    computed = n;
    return a_seq[n];
}
