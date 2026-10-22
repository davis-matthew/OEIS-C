/*
 * A372471  Irregular triangle read by rows where row n lists the binary
 *          indices of the n-th prime number.
 * https://oeis.org/A372471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating successive primes and collecting the
 * binary indices (positions of 1‑bits) of each prime's reversed
 * binary representation (ie. reading the binary digits from LSB to
 * MSB).  The values are flattened row‑by‑row into a single sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache of the sequence values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of cache */
    static size_t len = 0;   /* number of stored elements */

    /* Static state for prime generation */
    static idx_t candidate = 2;  /* current integer to test for primality */

    if (len < n) {
        /* Continue generating primes until we have at least n entries */
        while (len < n) {
            /* Find next prime */
            while (!is_prime(candidate))
                candidate++;

            /* Scan bits of the prime (LSB first) */
            idx_t tmp = candidate;
            idx_t pos = 1;         /* positions start at 1 */
            while (tmp != 0) {
                if (tmp & 1ULL) {
                    /* Grow cache if needed */
                    if (len == cap) {
                        size_t new_cap = (cap ? cap * 2 : 16);
                        seq_t *tmp_cache = (seq_t *)realloc(cache,
                                                             new_cap * sizeof(seq_t));
                        if (!tmp_cache)
                            exit(EXIT_FAILURE);  /* allocation failure */
                        cache = tmp_cache;
                        cap = new_cap;
                    }
                    cache[len++] = (seq_t)pos;
                }
                tmp >>= 1;
                pos++;
            }
            /* Advance to the next integer for future prime checks */
            candidate++;
        }
    }

    return cache[n - 1];
}
