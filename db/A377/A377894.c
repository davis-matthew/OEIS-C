/*
 * A377894  E.g.f. satisfies A(x) = (1 + x) * exp(x * A(x)^2).
 * https://oeis.org/A377894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (2*k+1)^(k-1) * binomial(2*k+1, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        (where 1^(-1) is defined as 1 for k=0) */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* memoised transient caches */
static seq_t *pow_cache  = NULL;    /* pow_cache[k] = (2*k+1)^(k-1) */
static size_t pow_cap    = 0;
static seq_t *fact_cache = NULL;    /* fact_cache[i] = i! */
static size_t fact_cap   = 0;
static seq_t *a_cache    = NULL;    /* a_cache[i] = a(i) */
static size_t a_cap      = 0;

/* Expand a cache up to idx */
static void ensure_pow_cache(idx_t n)
{
    if ((idx_t)pow_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(pow_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        pow_cache = tmp;
        for (size_t k = pow_cap; k <= n; ++k) {
            if (k == 0) {
                pow_cache[0] = (seq_t)1;   /* 1^(-1) == 1 */
            } else {
                pow_cache[k] = big_pow((seq_t)(2 * k + 1),
                                       (seq_t)(k - 1));
            }
        }
        pow_cap = new_cap;
    }
}

static void ensure_fact_cache(idx_t n)
{
    if ((idx_t)fact_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fact_cache = tmp;
        if (fact_cap == 0) {
            fact_cache[0] = (seq_t)1;
            fact_cap = 1;
        }
        for (size_t i = fact_cap; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }
}

static void ensure_a_cache(idx_t n)
{
    if ((idx_t)a_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;
        for (size_t m = a_cap; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t nfact = fact_cache[m];
            for (size_t k = 0; k <= m; ++k) {
                /* binomial automatically returns 0 when k > n-k,
                   so no need for a condition here */
                seq_t bin = binomial(2 * k + 1, m - k);
                if (bin == (seq_t)0) continue;
                seq_t factor = nfact / fact_cache[k];
                seq_t term = pow_cache[k];
                term = term * bin;
                term = term * factor;
                sum = sum + term;
            }
            a_cache[m] = sum;
        }
        a_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_pow_cache(n);
    ensure_fact_cache(n);
    ensure_a_cache(n);
    return a_cache[n];
}
