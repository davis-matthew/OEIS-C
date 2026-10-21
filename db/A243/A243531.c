/*
 * A243531  Prime numbers containing the string 15.
 * https://oeis.org/A243531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_15, ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose decimal representation contains the substring "15". */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"
#include "../utils/contains_15.h"

seq_t a(idx_t n)
{
    /* Static state: cached primes containing "15" and search progress. */
    static seq_t *cache = NULL;          /* array of primes containing "15" */
    static idx_t count = 0;              /* number of primes stored */
    static idx_t cap   = 0;              /* current capacity of the cache */
    static seq_t cur   = 1;              /* last tested integer */

    /* Grow the cache until it contains at least n elements. */
    while (count < n) {
        /* Advance to the next candidate, skipping even integers > 2. */
        do {
            cur++;
        } while (cur % 2 == 0 && cur != 2);

        /* Test the candidate. */
        if (contains_15(cur) && isPrimeGeneric(cur)) {
            /* Ensure capacity for a new element. */
            ensure_capacity((size_t)(count + 1), &cap,
                            (void **)&cache, sizeof(seq_t));
            cache[count++] = cur;
        }
    }
    return cache[n - 1];
}
