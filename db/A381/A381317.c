/*
 * A381317  Numbers of the form p^(p +- 1), where p is prime.
 * https://oeis.org/A381317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 147 (2026-11-03)
 * Helpers:   build_primes_upto, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381317: a(n) = the n‑th smallest number of the form p^(p-1) or p^(p+1), p prime (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <string.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

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
        seq_t low  = ipow((seq_t)p, (seq_t)(p - 1));
        seq_t high = ipow((seq_t)p, (seq_t)(p + 1));
        vals[idx++] = low;
        vals[idx++] = high;
    }

    /* ---- sort values ----------------------------------------------------- */
    mem_qsort(vals, total_vals, sizeof(seq_t), cmp_seq_t);

    /* ---- store in cache and free old data -------------------------------- */
    if (cache)
        free_seq_array(cache);

    cache = vals;
    cache_len = total_vals;

    free(primes);

    return cache[n - 1];
}
