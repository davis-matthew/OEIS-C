/*
 * A386507  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} (1 + k) * k^4 *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A386507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0..n-1} (1 + k) * k^4 *
 *            binomial(n-1,k) * a(k) * a(n-1-k) Offset: 0 Index: a(n) is
 *            returned for n = 0,1,
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed a(i) */
static idx_t cache_size = 0;     /* allocated length of *cache */
static idx_t last_computed = 0;  /* largest index that has been computed */

/* Ensure the cache can hold indices 0..n inclusive.
   Newly allocated entries are zeroed.  The base value a(0)=1 is set
   when the cache is first extended.                                  */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure – terminate the program */
        exit(EXIT_FAILURE);
    }
    /* initialize new entries to 0 */
    for (idx_t i = cache_size; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_cap;

    if (new_cap > 0 && cache[0] == 0)
        cache[0] = (seq_t)1;        /* base case a(0)=1 */
}

/* Return the n-th term of the sequence.  The function is memoised;
   it works for arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) {
        ensure_cache(0);
        return cache[0];
    }

    ensure_cache(n);

    /* If the value has already been computed, simply return it. */
    if (last_computed >= n)
        return cache[n];

    /* Make sure the base term is initialised */
    if (cache[0] == 0)
        cache[0] = (seq_t)1;

    /* Compute a(m) for all m from last_computed+1 up to n inclusive. */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;      /* binomial(m-1,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* Update binom to binomial(m-1,k) for k>0. */
            if (k > 0) {
                seq_t numer = (seq_t)(m - 1 - (k - 1));
                binom *= numer;
                binom /= (seq_t)k;
            }

            if (k == 0)      /* k^4 is zero, skip the term */
                continue;

            seq_t k_seq = (seq_t)k;
            seq_t weight = k_seq;                      /* k */
            weight *= k_seq;                            /* k^2 */
            weight *= k_seq;                            /* k^3 */
            weight *= k_seq;                            /* k^4 */
            weight *= (k_seq + 1);                      /* (1+k)*k^4 */
            weight *= binom;                            /* with binomial */

            seq_t term = cache[k];
            term *= cache[m - 1 - k];
            term *= weight;

            sum += term;
        }

        cache[m] = cache[m - 1] + sum;
    }

    last_computed = n;
    return cache[n];
}
