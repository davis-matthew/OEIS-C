/*
 * A351833  Number of partitions of n into exactly two parts, at least one of
 *          which is squarefree.
 * https://oeis.org/A351833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} [ is_squarefree(k) |
 *            is_squarefree(n-k) ]
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(idx_t n) returns the number of partitions of n into two parts,
   at least one of which is square‑free.  The calculation is done by
   pre‑computing the square‑free indicator (mu(k)^2) up to the largest
   requested n and then summing the indicators for each pair {k, n‑k}. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utilities provided by the build environment. */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &primes, &cnt) */

/* ------------------------------------------------------------------ */
/*  Square‑free table (mu(k)^2) and cache handling                       */
/* ------------------------------------------------------------------ */
static unsigned char *mu2_cache = NULL; /* 0 = non‑squarefree, 1 = squarefree */
static idx_t mu2_cap = 0;               /* largest index currently cached */

/* Ensure that mu2_cache[0..n] is populated.  If n is larger than the
   current capacity, extend the table and recompute the square‑free
   indicators for the new range.  This function is thread‑unsafe but is
   sufficient for a single‑threaded OEIS evaluation harness. */
static void
ensure_mu2_cache(idx_t n)
{
    if (n <= mu2_cap)
        return; /* already computed */

    /* Reallocate cache for indices 0..n.  Use a temporary pointer first
       to avoid memory leaks on allocation failure. */
    unsigned char *tmp = (unsigned char *)realloc(mu2_cache,
                                                  (size_t)(n + 1) * sizeof(unsigned char));
    if (!tmp)
        abort(); /* unrecoverable failure */

    /* For any newly added indices, initialise them to square‑free (1). */
    for (idx_t i = mu2_cap + 1; i <= n; ++i)
        tmp[i] = 1u;

    mu2_cache = tmp;

    /* Compute the list of primes up to n.  The helper function allocates
       an array that we must free after use. */
    size_t primes_cnt = 0;
    uint64_t *primes_arr = NULL;
    build_primes_upto(n, &primes_arr, &primes_cnt);

    /* Mark multiples of p^2 as non‑squarefree. */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes_arr[pi];
        uint64_t p_sq = p * p;
        if (p_sq > (uint64_t)n)
            break;
        idx_t j = (idx_t)p_sq;
        for (; j <= n; j += (idx_t)p_sq)
            mu2_cache[j] = 0u;
    }
    free(primes_arr);

    mu2_cap = n;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* The sequence is defined for n >= 1; above
                            offset is irrelevant for the harness. */

    /* Make sure square‑free data up to n is available. */
    ensure_mu2_cache(n);

    /* Sum indicator over k = 1..floor(n/2). */
    seq_t count = (seq_t)0;
    idx_t limit = n >> 1; /* floor(n/2) */
    for (idx_t k = 1; k <= limit; ++k) {
        /* mu2_cache elements are 0 or 1; the bitwise OR gives 0 or 1. */
        count += mu2_cache[k] | mu2_cache[n - k];
    }
    return count;
}
