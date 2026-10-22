/*
 * A365032  E.g.f. satisfies A(x) = exp(x * A(x) * (1 + x * A(x))^3).
 * https://oeis.org/A365032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+1)^(k-1) * binomial(3*k, n-k)/k!
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *a_cache   = NULL;
static seq_t *fac_cache = NULL;
static size_t cap       = 0;
static idx_t  a_max     = 0;
static idx_t  fac_max   = 0;

/* Ensure the arrays are large enough for indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n < cap) return;
    size_t new_cap = (cap > 0 ? cap : 1);
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_a = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    seq_t *new_f = (seq_t*)realloc(fac_cache, new_cap * sizeof(seq_t));
    if (!new_a || !new_f) abort();  /* allocation failure */

    /* Initialise any newly allocated slots. */
    if (a_cache == NULL) {
        for (idx_t i = 0; i < new_cap; ++i) {
            new_a[i] = (seq_t)0;
            new_f[i] = (seq_t)0;
        }
    } else {
        for (idx_t i = cap; i < new_cap; ++i) {
            new_a[i] = (seq_t)0;
            new_f[i] = (seq_t)0;
        }
    }

    a_cache = new_a;
    fac_cache = new_f;
    cap = new_cap;

    /* Initialise index 0 if this is the first allocation. */
    if (fac_max == 0 && a_max == 0) {
        a_cache[0] = (seq_t)1;
        fac_cache[0] = (seq_t)1;
        a_max = fac_max = 0;
    }
}

/* Extend the factorial cache up to n. */
static void ensure_factorial(idx_t n)
{
    if (n <= fac_max) return;
    for (idx_t i = fac_max + 1; i <= n; ++i) {
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;
    }
    fac_max = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n <= a_max) return a_cache[n];
    if (n == 0) return (seq_t)1;

    ensure_factorial(n);

    seq_t sum = 0;
    /* ceil(n/4) – the first k for which binomial(3k, n-k) is non‑zero. */
    idx_t k_start = (n + 3) / 4;
    seq_t pow_val = 1;                 /* (n+1)^(k-1) for current k */
    seq_t step    = (seq_t)(n + 1);
    seq_t fac_ratio = fac_cache[n];    /* n! / k!  (will be divided by k in loop) */
    seq_t binom = 0;                   /* current value of binomial(3k, n-k) */

    for (idx_t k = 1; k <= n; ++k) {
        fac_ratio /= k;                /* fac[n] / k! */

        if (k >= k_start) {
            if (k == k_start) {
                binom = binomial((idx_t)(3 * k), (idx_t)(n - k));
            } else {
                seq_t num = (seq_t)(3 * k) * (seq_t)(3 * k - 1) *
                            (seq_t)(3 * k - 2) * (seq_t)(n - k + 1);
                seq_t den = (seq_t)(4 * k - n - 3) * (seq_t)(4 * k - n - 2) *
                            (seq_t)(4 * k - n - 1) * (seq_t)(4 * k - n);
                binom = binom * num / den;
            }
            seq_t term = fac_ratio * pow_val * binom;
            sum += term;
        }
        pow_val *= step;               /* prepare (n+1)^(k) for next k */
    }

    a_cache[n] = sum;
    if (n > a_max) a_max = n;
    return sum;
}
