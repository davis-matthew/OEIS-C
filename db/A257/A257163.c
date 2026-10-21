/*
 * A257163  Primes of the form 3n^2 + 2.
 * https://oeis.org/A257163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime of the form 3*k^2 + 2 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 19, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* Static table of found primes */
static seq_t *primes_cache = NULL;   /* array of primes found so far   */
static size_t primes_len   = 0;      /* number of stored primes        */
static size_t primes_cap   = 0;      /* allocated capacity             */
static idx_t  last_checked = 0;      /* last k examined (inclusive)    */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; caller must use n >= 1 */
        return 0;

    /* Grow cache until at least n primes are available */
    while (primes_len < n) {
        /* Compute candidate = 3*k^2 + 2 */
        uint64_t k   = (uint64_t)last_checked;
        uint64_t cand = 3ULL * k * k + 2ULL;
        seq_t candidate = (seq_t)cand;

        if (is_prime_big(candidate)) {
            /* Ensure there is room in the table */
            ensure_capacity(primes_len + 1, &primes_cap,
                            (void **)&primes_cache, sizeof(seq_t));
            primes_cache[primes_len++] = candidate;
        }
        ++last_checked;
    }

    return primes_cache[n - 1];
}
