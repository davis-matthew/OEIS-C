/*
 * A305829  Factor n into distinct Fermi-Dirac primes (A050376), normalize by
 *          replacing every instance of the k-th Fermi-Dirac prime with k,
 *          then multiply everything together.
 * https://oeis.org/A305829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of the indices of the Fermi‑Dirac primes whose
 *            values multiply to n (sequence A305829)
 * Range:     u8 n <= 854, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 65, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static state for the precomputed set of Fermi‑Dirac primes,
       mapping from value to its index, and memoisation cache. */
    static idx_t   precomp_limit = 0;  /* largest n for which data was prepared */
    static uint32_t *irred_vals   = NULL;    /* ascending list of f(s) values */
    static uint32_t *val_to_idx   = NULL;    /* val_to_idx[v] = index of v in irred_vals, 0 if not irreducible */
    static size_t   irred_count   = 0;       /* number of irreducibles found */
    static seq_t  *cache         = NULL;    /* memoised a(k), indexed by k (1‑based) */
    static size_t  cache_cap     = 0;        /* allocation size of cache */

    /* Helper to grow the cache array to at least size 'needed'. */
    if (n >= cache_cap)
    {
        size_t new_cap = ((size_t)n) + 1;
        if (cache != NULL)
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Return memoised result if available.  a(1) is defined to be 1. */
    if (n > 0 && cache[n] != 0)
        return cache[n];

    /* If a(1) is requested, return 1 immediately. */
    if (n == 1)
    {
        cache[1] = (seq_t)1;
        return (seq_t)1;
    }

    /* --------------------------------------------------------------------
       Prepare the irreducible list up to 'n' if necessary.
       -------------------------------------------------------------------- */
    if (n > precomp_limit)
    {
        uint32_t limit = (uint32_t)n;   /* limit fits in 32 bits for the given bounds */

        /* Free any previously allocated structures. */
        if (irred_vals) free(irred_vals);
        if (val_to_idx) free(val_to_idx);

        /* 1.  Boolean array: 'used[v]' tells whether v can be expressed
              as a product of distinct previously seen irreducibles.  */
        uint8_t *used = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
        used[1] = 1;        /* 1 is trivially reachable (empty product). */

        /* 2.  Vector of all numbers reachable so far (distinct products). */
        size_t  reach_cap = 64, reach_len = 1;
        uint32_t *reachable = (uint32_t *)malloc(reach_cap * sizeof(uint32_t));
        reachable[0] = 1;   /* start with 1 */

        /* 3.  Temporary vector to hold new products when adding an irreducible. */
        size_t  temp_cap = 64, temp_len = 0;
        uint32_t *new_reach = (uint32_t *)malloc(temp_cap * sizeof(uint32_t));

        /* 4.  Growing vector for the irreducible list. */
        size_t   vals_cap = 64, vals_len = 0;
        uint32_t *vals = (uint32_t *)malloc(vals_cap * sizeof(uint32_t));

        /* Main loop: scan every integer up to the limit. */
        for (uint32_t v = 2; v <= limit; ++v)
        {
            if (!used[v]) /* v is irreducible */
            {
                /* store in the irreducible list */
                if (vals_len == vals_cap) {
                    vals_cap *= 2;
                    vals = (uint32_t *)realloc(vals, vals_cap * sizeof(uint32_t));
                }
                vals[vals_len++] = v;

                /* generate all new products that use this irreducible. */
                temp_len = 0;
                for (size_t i = 0; i < reach_len; ++i)
                {
                    uint64_t prod = (uint64_t)reachable[i] * v;
                    if (prod > limit)
                        continue;
                    uint32_t p = (uint32_t)prod;
                    if (!used[p]) {
                        used[p] = 1;
                        if (temp_len == temp_cap) {
                            temp_cap *= 2;
                            new_reach = (uint32_t *)realloc(new_reach, temp_cap * sizeof(uint32_t));
                        }
                        new_reach[temp_len++] = p;
                    }
                }

                /* Append new products to the reachable list. */
                if (reach_len + temp_len > reach_cap) {
                    reach_cap = reach_len + temp_len + 64;
                    reachable = (uint32_t *)realloc(reachable, reach_cap * sizeof(uint32_t));
                }
                for (size_t i = 0; i < temp_len; ++i)
                    reachable[reach_len++] = new_reach[i];
            }
        }

        /* Clean up temporary arrays. */
        free(used);
        free(new_reach);
        free(reachable);

        /* Store the final data for future calls. */
        irred_vals = vals;
        irred_count = vals_len;
        precomp_limit = limit;

        /* Build quick index lookup: val_to_idx[value] = 1‑based position in irred_vals. */
        val_to_idx = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        for (size_t i = 0; i < vals_len; ++i)
            val_to_idx[ vals[i] ] = (uint32_t)(i + 1);   /* indices start at 1 */
    }

    /* --------------------------------------------------------------------
       Compute a(n) recursively using the largest irreducible divisor.
       -------------------------------------------------------------------- */
    uint32_t idx = 0;        /* index of the chosen irreducible (1‑based) */
    for (size_t i = irred_count; i > 0; --i)
    {
        uint32_t val = irred_vals[i - 1];
        if (val > n)
            continue;
        if (n % val == 0)  {
            idx = (uint32_t)i;
            break;
        }
    }

    /* idx must be non‑zero for n > 1 (the sequence guarantees a factorization). */
    idx_to_seq_t:
    ;
    if (idx == 0) {
        /* This should never happen for n >= 2.  Return 1 as a fallback. */
        cache[n] = (seq_t)1;
        return cache[n];
    }

    idx_t sub_n = n / irred_vals[idx - 1];
    seq_t sub_a = a(sub_n);          /* recursive call handles memoisation */
    seq_t result = (seq_t)idx * sub_a;

    cache[n] = result;
    return result;
}
