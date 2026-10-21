/*
 * A282323  Lesser of twin primes congruent to 17 (mod 30).
 * https://oeis.org/A282323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p and p+2 are prime and p % 30
 *            == 17
 * Range:     u8 n <= 5, u16 n <= 283, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

/* Static cache for the sequence values */
static seq_t *cache = NULL;         /* dynamic array of found terms   */
static size_t cache_len = 0;        /* number of terms stored         */
static size_t cache_cap = 0;        /* current capacity of the array  */

/* Next candidate with residue 17 mod 30 to test */
static seq_t next_candidate = 17;   /* starts at the first possible term */

/* Return the nth element of sequence A282323 (offset 1). */
seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 for safety. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n entries. */
    while (cache_len < n) {
        /* Test whether the current candidate and its twin are prime. */
        if (isPrimeGeneric(next_candidate) &&
            isPrimeGeneric(next_candidate + 2)) {
            /* Append candidate to the cache. */
            append_candidate(&cache, &cache_len, &cache_cap,
                             next_candidate);
        }
        /* Move to the next residue 17 mod 30. */
        next_candidate += 30;
    }

    /* Return the required element (array is zero‑indexed). */
    return cache[n - 1];
}
