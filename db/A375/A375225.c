/*
 * A375225  Expansion of e.g.f. exp( x^2/(1-x)^3 ) / (1-x)^3.
 * https://oeis.org/A375225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} binomial(n+2+k,n-2*k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static seq_t *cache = NULL;    /* cache[0..cache_len-1] = a(i) */
static idx_t  cache_len = 0;   /* number of stored terms */

/* compute a(t+1) from a(t), a(t-1), a(t-2), a(t-3)
   recurrence derived from the e.g.f. of the sequence: */
static seq_t compute_next(idx_t t)
{
    seq_t term1, term2, term3, term4;
    seq_t result = 0;

    /* (4t+3) * a(t) */
    term1 = cache[t] * (seq_t)(4 * t + 3);
    result += term1;

    /* – t(6t+1) * a(t‑1)  when t ≥ 1 */
    if (t > 0) {
        term2 = cache[t - 1] * (seq_t)(t * (6 * t + 1));
        result -= term2;
    }

    /* + 2t(t‑1)(2t+1) * a(t‑2)  when t ≥ 2 */
    if (t > 1) {
        term3 = cache[t - 2] * (seq_t)(2 * t * (t - 1) * (2 * t + 1));
        result += term3;
    }

    /* – t²(t‑1)(t‑2) * a(t‑3)  when t ≥ 3 */
    if (t > 2) {
        term4 = cache[t - 3] * (seq_t)(t * t * (t - 1) * (t - 2));
        result -= term4;
    }

    return result;
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    /* Initialise cache with a(0) if necessary */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure */
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        cache_len = 1;
    }

    /* Extend cache up to the requested index n */
    while (cache_len <= n) {
        idx_t new_len = cache_len + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                          /* realloc failed */
            free(cache);
            cache = NULL;
            cache_len = 0;
            return 0;
        }
        cache = tmp;
        cache[cache_len] = compute_next(cache_len - 1);
        cache_len = new_len;
    }

    return cache[n];
}
