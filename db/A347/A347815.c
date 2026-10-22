/*
 * A347815  Prime numbers p such that both 30 and 105 are quadratic nonresidue
 *          (mod p).
 * https://oeis.org/A347815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that (30|p) = -1 and (105|p) = -1
 *            (Eulerian polynomial E5 irreducible mod p). The code searches
 *            primes sequentially and caches found terms. It uses the helper
 *            functions is_prime(idx_t) and jacobi_symbol(uint64_t,uint64_t)
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of primes that satisfy the condition */
static size_t cache_cap = 0;  /* current allocated capacity of cache */
static idx_t cache_len = 0;   /* number of terms stored in cache */
static uint64_t next_candidate = 3;  /* next odd number to test for primality */

/* Ensure that the cache can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Find the next prime candidate. */
        while (!is_prime((idx_t)next_candidate))
            next_candidate += 2; /* skip composites and even numbers */

        /* Check the quadratic non‑residue conditions. */
        if (jacobi_symbol((uint64_t)30, next_candidate) == -1 &&
            jacobi_symbol((uint64_t)105, next_candidate) == -1) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        next_candidate += 2;      /* advance to the following odd number */
    }

    return cache[n - 1];
}
