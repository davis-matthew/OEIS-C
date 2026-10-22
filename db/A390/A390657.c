/*
 * A390657  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 2 * x * (1 + x +
 *          x^2) * A(x^3))).
 * https://oeis.org/A390657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * Sum_{k=0..n-1} a(floor(k/3)) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Cached values of the sequence                                    */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;   /* array of computed terms            */
static size_t cache_cap = 0;     /* current allocated capacity        */
static size_t cache_len = 0;     /* number of terms that are known    */

/* ------------------------------------------------------------------ */
/*  Ensure that cache has capacity for at least 'needed' elements    */
/* ------------------------------------------------------------------ */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) {
        /* In a real library we would handle memory exhaustion more gracefully.
           For the OEIS evaluation we can simply abort. */
        abort();
    }

    cache     = new_arr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Compute all terms up to index n (inclusive) and return the array */
/* ------------------------------------------------------------------ */
static seq_t *compute_up_to(idx_t n)
{
    /* n is unsigned; cast to size_t (assume it fits) */
    size_t idx = (size_t)n;

    if (idx < cache_len)
        return cache;

    ensure_capacity(idx + 1);

    for (size_t cur = cache_len; cur <= idx; ++cur) {
        if (cur == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (size_t k = 0; k < cur; ++k) {
                sum += cache[k / 3] * cache[cur - 1 - k];
            }
            cache[cur] = 1 + sum + sum;   /* 1 + 2*sum */
        }
    }

    cache_len = idx + 1;
    return cache;
}

/* ------------------------------------------------------------------ */
/*  Public API: returns a(n) for any non‑negative n                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    return compute_up_to(n)[(size_t)n];
}
