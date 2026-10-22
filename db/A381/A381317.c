/*
 * A381317  Numbers of the form p^(p +- 1), where p is prime.
 * https://oeis.org/A381317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto, cmp_seq_t,
 *            free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381317: a(n) = the n‑th smallest number of the form p^(p-1) or p^(p+1), p prime (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Return the n‑th term of the sequence
   (n is 1‑based).  Calls to a() with decreasing
   n are allowed; cached results are reused. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* outside offset – defined as 0 for safety */

    if (n <= cache_len)
        return cache[n - 1];

    /* ---- set up primes ---------------------------------------------------- */
    /* We need ceil(n/2) primes to supply 2*ceil(n/2) candidate values. */
    idx_t primes_needed = (n + 1) / 2;

    /* Find an upper bound for the largest required prime. */
    size_t prime_limit = _prime_limit_for_idx(primes_needed);

    uint64_t *primes = NULL;
    size_t prime_count = 0;
    build_primes_upto((idx_t)prime_limit, &primes, &prime_count);

    /* In the unlikely event that the limit was too low, retry with a larger one. */
    if (prime_count < (size_t)primes_needed)
    {
        size_t new_limit = _prime_limit_for_idx(primes_needed * 2);
        free(primes);
        build_primes_upto((idx_t)new_limit, &primes, &prime_count);
    }

    /* ---- generate values ------------------------------------------------- */
    size_t total_vals = prime_count * 2;
    seq_t *vals = seq_alloc_new_array(total_vals);

    size_t idx = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = primes[i];
        seq_t low  = big_pow((seq_t)p, (seq_t)(p - 1));
        seq_t high = big_pow((seq_t)p, (seq_t)(p + 1));
        vals[idx++] = low;
        vals[idx++] = high;
    }

    /* ---- sort values ----------------------------------------------------- */
    qsort(vals, total_vals, sizeof(seq_t), cmp_seq_t);

    /* ---- store in cache and free old data -------------------------------- */
    if (cache)
        free_seq_array(cache);

    cache = vals;
    cache_len = total_vals;

    free(primes);

    return cache[n - 1];
}
