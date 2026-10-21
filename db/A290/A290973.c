/*
 * A290973  Write 2*x/(1-x) in the form Sum_{j>=1} ((1-x^j)^a(j) - 1).
 * https://oeis.org/A290973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{d|n, d<n} binomial(-a(d)+n/d-1 , n/d) - 2
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 28, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from: 2 = sum_{d|n} binomial(-a(d)+n/d-1 , n/d)   */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached values -------------------------------------------------------- */
static seq_t *cache = NULL;   /* array of computed a(n)   */
static size_t cache_cap = 0;  /* capacity of cache array   */

/* Flags for computed indices --------------------------------------------*/
static bool *computed = NULL; /* true if cache[n] is valid   */
static size_t computed_cap = 0;/* capacity of computed array */

/* Ensure cache has room for index n -------------------------------------*/
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Ensure computed array has room for index n -----------------------------*/
static void ensure_computed(idx_t n)
{
    if (computed_cap <= n) {
        size_t old_cap = computed_cap;
        size_t new_cap = n + 1;
        bool *tmp = (bool *)realloc(computed, new_cap * sizeof(bool));
        if (!tmp) exit(1);
        for (size_t i = old_cap; i < new_cap; ++i)
            tmp[i] = false;
        computed = tmp;
        computed_cap = new_cap;
    }
}

/* Multiplicative binomial: binom(x,k) for integer k>=0, x may be negative */
static seq_t binom_seq_t(seq_t x, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (x - (seq_t)(i - 1));
        res = res / (seq_t)i;
    }
    return res;
}

/* Sequence function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    ensure_cache(n);
    ensure_computed(n);

    if (computed[n])
        return cache[n];

    if (n == 1) {                 /* base case */
        cache[1] = (seq_t)-2;
        computed[1] = true;
        return cache[1];
    }

    seq_t sum = 0;
    /* iterate over all proper divisors of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* first divisor d */
        idx_t d1 = d;
        if (d1 < n) {
            seq_t ad = a(d1);            /* ensures ad is cached */
            idx_t m  = n / d1;
            seq_t upper = (-ad) + (seq_t)m - 1;
            seq_t term = binom_seq_t(upper, m);
            sum += term;
        }

        /* paired divisor n/d if distinct */
        idx_t d2 = n / d;
        if (d2 != d1 && d2 < n) {
            seq_t ad = a(d2);            /* ensures ad is cached */
            idx_t m  = n / d2;
            seq_t upper = (-ad) + (seq_t)m - 1;
            seq_t term = binom_seq_t(upper, m);
            sum += term;
        }
    }

    seq_t an = sum - (seq_t)2;
    cache[n] = an;
    computed[n] = true;
    return an;
}
