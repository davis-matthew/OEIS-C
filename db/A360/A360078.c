/*
 * A360078  Moebius function for the floor quotient poset.
 * https://oeis.org/A360078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 9946, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = - Σ_{d∈Q(n), d≠n} a(d),
   where Q(n) = { ⌊n/k⌋ | k ≥ 1 }. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for μ values.  mu_cache[i] holds a(i) for i ≤ mu_n. */
static seq_t *mu_cache = NULL;
static idx_t  mu_cap   = 0;   /* capacity of mu_cache (max index stored + 1)   */
static idx_t  mu_n     = 0;   /* highest n that has been computed              */

/* Ensure that a(1)…a(n) are computed and stored in mu_cache. */
static void ensure_cache(idx_t n)
{
    if (n <= mu_n) return;            /* already up to date */

    /* Grow cache if necessary. */
    idx_t new_cap = mu_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap < n) new_cap <<= 1;              /* power‑of‑two grows */

    seq_t *tmp = (seq_t *)realloc(mu_cache, (new_cap + 1) * sizeof(seq_t));
    if (tmp == NULL) abort();                      /* out‑of‑memory protection */
    mu_cache = tmp;
    mu_cap   = new_cap;

    /* Compute missing values from mu_n+1 … n. */
    for (idx_t i = mu_n + 1; i <= n; ++i) {
        if (i == 1) {
            mu_cache[1] = 1;
        } else {
            seq_t sum = 0;
            idx_t l = 1;
            while (l <= i) {
                idx_t v = i / l;           /* distinct quotient */
                if (v != i) sum += mu_cache[v];
                idx_t r = i / v;
                l = r + 1;
            }
            mu_cache[i] = -sum;
        }
    }
    mu_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* indices start at 1 */
    ensure_cache(n);
    return mu_cache[n];
}
