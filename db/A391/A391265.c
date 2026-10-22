/*
 * A391265  a(n) = Sum_{k=0..floor(2*n/5)} binomial(k,2*n-5*k).
 * https://oeis.org/A391265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 50, u32 n <= 94, u64 n <= 182,
 *            u128 n <= 357, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-3) + a(n-5) - a(n-6)  (n ≥ 6)
   Base values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=1 */
#include <stdlib.h>

/* Cache for the computed sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;      /* allocated length of seq_cache         */
static idx_t  seq_computed_len = 0;   /* number of sequence values stored      */
static const seq_t init_vals[6] = {1, 0, 0, 1, 0, 1};

/* Return the nth term of OEIS A391265 */
seq_t a(idx_t n)
{
    /* Return already computed value if available */
    if (n < seq_computed_len)
        return seq_cache[(size_t)n];

    /* First call: allocate space and seed initial values */
    if (seq_computed_len == 0) {
        size_t new_cap = 16;
        while (new_cap < 6) new_cap <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache_len = new_cap;
        for (size_t i = 0; i < 6; ++i)
            seq_cache[i] = init_vals[i];
        seq_computed_len = 6;
    }

    /* Extend cache capacity if needed */
    if ((size_t)n >= seq_cache_len) {
        size_t new_len = seq_cache_len;
        while (new_len <= (size_t)n)
            new_len <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache, new_len * sizeof(seq_t));
        seq_cache_len = new_len;
    }

    /* Compute missing terms up to n */
    for (size_t i = seq_computed_len; i <= (size_t)n; ++i) {
        if (i < 6) {                     /* safety; already initialised */
            seq_cache[i] = init_vals[i];
            continue;
        }
        seq_t val = seq_cache[i - 3];
        val += val;                      /* 2 * a(i-3) */
        val += seq_cache[i - 5];
        val -= seq_cache[i - 6];
        seq_cache[i] = val;
    }
    seq_computed_len = (size_t)n + 1;

    return seq_cache[(size_t)n];
}
