/*
 * A369966  Numbers of the form 4m+2 that have an even number of prime factors
 *          with multiplicity.
 * https://oeis.org/A369966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 8301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th number of the form 4m+2 that has an even count
   of prime factors with multiplicity (counting multiplicity).  The function
   generates terms by incremental search; each candidate is tested with
   the helper count_prime_factors.  Results are cached so that calls with
   arbitrary n are answered in O(1) after the necessary terms have been
   generated.  The implementation is efficient for n up to ~10⁴. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"

/* Static cache for generated terms. */
static seq_t *cache      = NULL;   /* array of computed values        */
static size_t cache_cap  = 0;      /* current capacity of the array   */
static idx_t  terms_found = 0;     /* number of terms already stored  */
static seq_t  next_candidate = 6;  /* first integer of the form 4m+2 */

/* Ensure that `cache` has room for at least `n` terms.  Term generation
   proceeds incrementally, stopping when we have `n` terms stored. */
static void ensure_cache(idx_t n)
{
    while (terms_found < n) {
        /* Count prime factors (with multiplicity) of the current candidate. */
        seq_t omega = count_prime_factors(next_candidate);

        /* If the total number of prime factors is even, store the candidate. */
        if ((omega & 1U) == 0) {
            if (cache_cap == terms_found) {
                /* Grow the cache: double its capacity, or start at 8. */
                size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* In case of allocation failure, terminate. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[terms_found] = next_candidate;
            terms_found++;
        }

        /* Move to the next number of the form 4m+2. */
        next_candidate += 4;
    }
}

/* Return the n‑th term (n >= 1).  Behaviour for n == 0 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined, but guard against accidental calls */
    ensure_cache(n);
    return cache[n - 1];
}
