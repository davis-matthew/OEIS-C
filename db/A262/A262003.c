/*
 * A262003  L.g.f.: log( Sum_{n>=0} x^n/n! * Product_{k=1..n} (k^3 + 1) ).
 * https://oeis.org/A262003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = f1 For n >= 2: a(n) = n*f_n - Σ_{k=1}^{n-1} a(k) *
 *            f_{n-k} where f_0 = 1 and f_n = f_{n-1} * (n^3+1) / n (always
 *            integer)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_free.h"

/* Eager allocation of an array of seq_t */
static seq_t *seq_alloc(size_t n) {
    return (seq_t*)malloc(n * sizeof(seq_t));
}

/* Cached data for f[n] = B_n / n!  and a[n] */
static seq_t *f_cache = NULL;   /* f[n] values, 0 <= n <= max_used */
static seq_t *a_cache = NULL;   /* a[n] values, 0 <= n <= max_used */
static size_t  cache_cap   = 0;  /* current capacity of caches */
static idx_t   max_used    = 0;  /* maximum n that has been computed */

/* Ensure that a_cache[0..n] and f_cache[0..n] are valid. */
static void ensure_cache(idx_t n) {
    if (n <= max_used) return;

    /* Grow to at least n+1 entries (index 0..n) */
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */

    seq_t *new_f = f_cache;
    seq_t *new_a = a_cache;
    if (new_f) new_f = (seq_t*)realloc(new_f, new_cap * sizeof(seq_t));
    else       new_f = seq_alloc(new_cap);
    if (new_a) new_a = (seq_t*)realloc(new_a, new_cap * sizeof(seq_t));
    else       new_a = seq_alloc(new_cap);

    f_cache = new_f;
    a_cache = new_a;
    cache_cap = new_cap;

    /* Base case */
    if (max_used == 0) {
        f_cache[0] = (seq_t)1;  /* B_0 / 0! = 1 */
        a_cache[0] = (seq_t)0;  /* unused */
    }

    /* Compute values from max_used+1 up to n */
    for (idx_t i = max_used + 1; i <= n; ++i) {
        /* f[i] = f[i-1] * (i^3 + 1) / i */
        seq_t tmp = f_cache[i-1] / (seq_t)i;
        seq_t cube_plus = ((seq_t)i * (seq_t)i * (seq_t)i) + 1;
        f_cache[i] = tmp * cube_plus;

        /* a[i] = i * f[i] - Σ_{k=1}^{i-1} a[k] * f[i-k] */
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += a_cache[k] * f_cache[i-k];
        }
        a_cache[i] = (seq_t)i * f_cache[i] - sum;
    }

    max_used = n;
}

/* Public function required by the specification. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* undefined for n<1, return 0 */
    ensure_cache(n);
    return a_cache[n];
}
