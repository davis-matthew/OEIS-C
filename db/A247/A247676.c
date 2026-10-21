/*
 * A247676  Odd composite numbers congruent to 2 modulo 9.
 * https://oeis.org/A247676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = the nth odd composite number with a(n) ≡ 11 (mod 18)
 *            (i.e., odd, ≡ 2 (mod 9), and composite)
 * Range:     u8 n <= 5, u16 n <= 1951, u32 n <= 4204, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Cached terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t  cache_len = 0;   /* number of terms stored in cache */

/* Next value of t to test (0‑based). Numbers are 18*t+11. */
static idx_t next_t = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offsets below 1, harmless */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* expand cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < n)
                new_cap = n;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* candidate of the form 18*t + 11 */
        seq_t cand = (seq_t)18 * (seq_t)next_t + (seq_t)11;

        /* test for compositeness: not prime and >1 (always true here) */
        if (!is_prime_big(cand)) {
            cache[cache_len++] = cand;
        }
        ++next_t;
    }

    return cache[n - 1];
}
