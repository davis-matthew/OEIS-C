/*
 * A348207  Product_{n>=1} (1 + a(n)*x^n/(n!)^2) = BesselI(0,2*sqrt(x)).
 * https://oeis.org/A348207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Product_{n>=1} (1 + a(n)*x^n/(n!)^2) = BesselI(0,2*sqrt(x)) =
 *            Sum_{n>=0} x^n/(n!)^2 Recurrence: let W[m] = (m!)^2 *
 *            [x^m]∏_{j<k} (1 + a(j)*x^j/(j!)^2). Then a(k) = 1 - W[k].
 *            Updating rule: W'[m] = W[m] + a(k) * C(m,k)^2 * W[m-k] (seq_t
 *            arithmetic) (Here C(m,k) = binomial(m,k).)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   binomial, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *a_cache   = NULL;   /* cache of a(n) values (index 0 unused) */
static idx_t  max_computed = 0;   /* largest n for which a_cache is valid */

/* Recompute all terms up to and including n, storing them in a_cache. */
static void recompute_up_to(idx_t n)
{
    size_t cap = (size_t)n + 1;
    seq_t *new_cache = seq_alloc_new_array(cap);
    seq_t *W = seq_alloc_new_array(cap);

    /* Initialise arrays */
    for (size_t i = 0; i < cap; ++i) {
        new_cache[i] = 0;
        W[i] = 0;
    }
    W[0] = 1;          /* 0!^2 · [x^0] = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t w_k = W[(size_t)k];          /* scaled T_k = (k!)^2 * T_k */
        seq_t a_k = (seq_t)1 - w_k;        /* a(k) = 1 – W[k] */
        new_cache[(size_t)k] = a_k;

        /* Start with C(n,k) and walk downwards so that C(m-1,k) can be
           derived from C(m,k) via C(m-1,k) = C(m,k) * (m - k) / m. */
        seq_t binom = binomial((idx_t)n, (idx_t)k);

        for (idx_t m = n; m >= k; --m) {
            seq_t add;
            if (W[(size_t)(m - k)] != 0) {
                seq_t bin_sq = binom * binom;            /* C(m,k)^2 */
                add = a_k * bin_sq * W[(size_t)(m - k)];
                W[(size_t)m] += add;
            }
            if (m == k) break;   /* avoid division by zero */
            /* Update binom for next lower m: C(m-1,k) */
            binom = binom * (seq_t)(m - k) / (seq_t)m;
        }
    }

    if (a_cache) seq_free_array(a_cache);
    a_cache = new_cache;
    seq_free_array(W);
    max_computed = n;
}

/* Ensure that a_cache holds values up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed) return;
    recompute_up_to(n);
}

/* Return the n-th term of the sequence (n≥1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n=0; return 0 as a placeholder */
    ensure_cache(n);
    return a_cache[(size_t)n];
}
