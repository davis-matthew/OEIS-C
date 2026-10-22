/*
 * A366729  E.g.f. satisfies A(x) = 1 + log(1 + x*A(x)^2)/A(x).
 * https://oeis.org/A366729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (2*n-k)!/(2*n-2*k+1)! * Stirling1(n,k)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Global storage for sequence values and Stirling numbers */
static seq_t *a_cache = NULL;          /* a_cache[i] = a(i) */
static size_t a_cap   = 0;             /* current capacity of a_cache */

static seq_t *s       = NULL;          /* holds Stirling numbers of the first kind for current n */
static size_t s_cap   = 0;             /* current capacity of s array */

static idx_t computed_n = (idx_t)-1;   /* highest index for which a(n) is computed */

/* Ensure a_cache has at least 'need' slots. Preserve existing values. */
static void ensure_a(size_t need)
{
    if (need > a_cap) {
        seq_t *new_a = alloc_seq_array(need);
        for (size_t i = 0; i < a_cap; ++i) new_a[i] = a_cache[i];
        for (size_t i = a_cap; i < need; ++i) new_a[i] = 0;
        if (a_cache) free_seq_array(a_cache);
        a_cache = new_a;
        a_cap   = need;
    }
}

/* Ensure s has at least 'need' slots. Preserve existing values. */
static void ensure_s(size_t need)
{
    if (need > s_cap) {
        seq_t *new_s = alloc_seq_array(need);
        for (size_t i = 0; i < s_cap; ++i) new_s[i] = s[i];
        for (size_t i = s_cap; i < need; ++i) new_s[i] = 0;
        if (s) free_seq_array(s);
        s = new_s;
        s_cap = need;
    }
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    /* First-time initialization */
    if (a_cache == NULL) {
        ensure_a(1);              /* need at least a_cache[0] */
        a_cache[0] = 1;
        ensure_s(1);              /* need s[0] */
        s[0] = 1;
        computed_n = 0;
    }

    /* Return cached value if already computed */
    if (n <= computed_n) return a_cache[n];

    /* Ensure cache can hold up to n */
    ensure_a((size_t)n + 1);

    /* Compute from computed_n+1 up to n */
    for (idx_t i = computed_n + 1; i <= n; ++i) {
        ensure_s((size_t)i + 1);   /* s must hold indices up to i */

        /* Update Stirling numbers in-place (high to low to keep old values) */
        for (idx_t j = i; j > 0; --j) {
            s[j] = s[j - 1] - (seq_t)(i - 1) * s[j];
        }
        s[0] = 0;                  /* s(i,0) = 0 for i >= 1 */

        /* Compute a(i) = sum_{k=1..i} prod * s[k], where prod = (2i-k)!/(2i-2k+1)! */
        seq_t sum = 0;
        if (i == 0) {
            sum = 1;
        } else {
            for (idx_t k = 1; k <= i; ++k) {
                seq_t prod = 1;
                idx_t lo = 2 * i - 2 * k + 2;
                idx_t hi = 2 * i - k;
                for (idx_t t = lo; t <= hi; ++t) prod *= (seq_t)t;
                sum += prod * s[k];
            }
        }

        a_cache[i] = sum;
        computed_n = i;
    }

    return a_cache[n];
}
