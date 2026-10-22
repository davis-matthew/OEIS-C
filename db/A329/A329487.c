/*
 * A329487  Primes p such that not all decimal digits of p occur in p^3.
 * https://oeis.org/A329487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 276, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth prime p for which not all decimal digits of p occur in p³. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_frequencies.h"
#include "../utils/is_prime.h"

/* static cache for sequence values                         */
static seq_t *cache = NULL;   /* array of stored terms      */
static size_t cache_size = 0; /* number of terms stored     */
static size_t cache_cap   = 0;/* allocated capacity in cache*/
static idx_t last_checked = 2;/* next candidate for testing */

/* grow the cache capacity (double it or set to 16) */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort();
    if (cache) {
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* append a value to the cache */
static void append_cache(seq_t v)
{
    if (cache_size == cache_cap) grow_cache();
    cache[cache_size++] = v;
}

/* Return the nth term of A329487 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behavior below offset is irrelevant */

    /* extend the cache until it contains at least n terms */
    while (cache_size < n) {
        /* find the next prime starting from last_checked */
        idx_t p = last_checked;
        while (!is_prime(p)) ++p;
        last_checked = p + 1;

        /* compute p³ */
        seq_t sp   = (seq_t)p;
        seq_t pow3 = sp * sp * sp; /* seq_t multiplication, works up to 128 bit */

        /* digit frequencies of p³ */
        unsigned int freq[10] = {0};
        digit_frequencies(pow3, freq);

        /* check whether every digit of p appears in p³ */
        bool all_present = true;
        idx_t q = p;
        while (q > 0) {
            unsigned int d = (unsigned int)(q % 10);
            if (freq[d] == 0) {
                all_present = false;
                break;
            }
            q /= 10;
        }

        /* store p if it fails the test */
        if (!all_present) append_cache((seq_t)p);
    }

    return cache[n - 1];
}
