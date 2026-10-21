/*
 * A296520  Multidigit primes in which the largest digit appears only once.
 * https://oeis.org/A296520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_frequencies, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multidigit primes with the largest digit occurring exactly once.
   a(n) is the n‑th prime >= 10 whose largest decimal digit appears
   only once.  The sequence starts with 13, 17, 19, 23, 29 ...   */

/* The implementation maintains a cache of the primes already found
   and extends it lazily until at least n terms are present.  The
   detection of a qualifying prime uses `is_prime_big` and the
   frequency table supplied by `digit_frequencies`.  The algorithm
   has linear time in the index n and is valid for any `seq_t`
   implementation provided by the build. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_frequencies.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;            /* cached qualifying primes   */
static size_t cache_size = 0;          /* number of terms stored    */
static size_t cache_cap  = 0;          /* allocated capacity        */
static seq_t next_candidate = 10;      /* next candidate to test    */

/* Ensure that the cache holds at least n terms (n >= 1). */
static void ensure_cache_size(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* Search for the next suitable prime. */
        while (true) {
            if (next_candidate >= (seq_t)10 && is_prime_big(next_candidate)) {
                unsigned int freq[10] = {0};
                digit_frequencies(next_candidate, freq);

                /* Identify the largest digit present. */
                int max_digit = -1;
                for (int d = 9; d >= 0; --d) {
                    if (freq[d] > 0) {
                        max_digit = d;
                        break;
                    }
                }

                /* Olargest digit must appear exactly once. */
                if (max_digit >= 0 && freq[max_digit] == 1)
                    break;          /* found a qualifying prime */
            }
            ++next_candidate;           /* try the next integer   */
        }

        /* Store the qualifying prime in the cache. */
        ensure_capacity(cache_size + 1, &cache_cap,
                        (void **)&cache, sizeof(seq_t));
        cache[cache_size++] = next_candidate;

        /* Continue searching from the following integer. */
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for indices < 1 */

    ensure_cache_size(n);
    return cache[n - 1];
}
