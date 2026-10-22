/*
 * A301992  a(n) = 8*(n-2)*(2*n-5)*a(n-1) + ((n-2)/9)*Product_{k=0..n-2}
 *          (2*k-3)^2 with a(1) = 0.
 * https://oeis.org/A301992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*(n-2)*(2*n-5)*a(n-1) + ((n-2)/9)*∏_{k=0}^{n-2}(2*k-3)^2
 *            with a(1) = 0. The implementation uses the recurrence directly,
 *            storing values in a static cache so that calls may come in any
 *            order. All arithmetic is performed with type seq_t, which can be
 *            an unsigned integer of various sizes or...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache   = NULL;   /* table of computed a(n) values   */
static size_t cache_cap   = 0;      /* allocated capacity of seq_cache  */
static size_t cache_len   = 0;      /* number of values currently calculated (a(1)..a(cache_len)) */
static seq_t prod_val     = 1;      /* ∏_{k=0}^{(current_n)-2}(2*k-3)^2  */

/*  Return the n‑th term of the sequence (a(1) is the first term).  */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; 0 is never requested */
        return 0;

    /* Initialise cache on first call.  */
    if (seq_cache == NULL) {
        size_t init_cap = 2;
        seq_cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!seq_cache) abort();          /* terminate if allocation fails */

        seq_cache[0] = 0;                /* a(1) = 0   */
        cache_len   = 1;                 /* we have computed up to a(1)   */
        prod_val    = 1;                 /* P(1) = 1   */
        cache_cap   = init_cap;
    }

    /* Resize cache if necessary.  */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* If the requested value is already cached, return it.  */
    if ((size_t)n <= cache_len)
        return seq_cache[n-1];

    /*  Compute successive terms up to n (inclusive).  */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {

        /* Update the product ∏_{k=0}^{i-2}(2*k-3)^2               */
        int64_t v = (int64_t)2 * (int64_t)(i-1) - 5;   /* 2*(i-1)-5 may be negative */
        seq_t factor = (seq_t)(v * v);                 /* square, fits in 64 bits */
        prod_val = prod_val * factor;

        /*  term = ((i-2)/9) * prod_val     (exact division)     */
        seq_t term = ((seq_t)(i-2) * prod_val) / 9;

        /*  part1 = 8*(i-2)*(2*i-5) * a(i-1)                           */
        int64_t mul = 8 * (int64_t)(i-2) * ((int64_t)2 * (int64_t)i - 5);
        seq_t part1 = seq_cache[i-2] * (seq_t)mul;

        /*  Store the computed value.  */
        seq_cache[i-1] = part1 + term;
    }

    cache_len = (size_t)n;
    return seq_cache[n-1];
}
