/*
 * A271669  Smallest k > n such that n divides k and n^k == n (mod k).
 * https://oeis.org/A271669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n for odd n; for even n, a(n) = n * m where m is the
 *            least integer >1 such that n^(n*m-1) ≡ 1 (mod m). The value is
 *            found by a brute‑force search of the multiplier m
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;

/* Ensure the cache can hold indices up to n. */
static void a_ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= a_cache_size) return;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);          /* out of memory */
    /* Initialise new entries to 0 (uncomputed). */
    for (size_t i = a_cache_size; i < new_size; ++i) new_cache[i] = 0;
    a_cache = new_cache;
    a_cache_size = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0 */

    a_ensure_cache(n);
    if (a_cache[n] != 0) return a_cache[n];

    seq_t d = (seq_t)n;
    seq_t result;

    if ((n & 1) == 1) {                   /* odd n */
        result = d * 2;
    } else {                              /* even n */
        /* Search for the smallest m > 1 such that
           d^(d*m-1) ≡ 1 (mod m). */
        for (uint64_t m = 2;; ++m) {
            seq_t mod = (seq_t)m;
            /* Exponent: d*m-1.  Use 128-bit arithmetic to avoid overflow. */
            unsigned __int128 prod = (unsigned __int128)d * m;
            uint64_t exponent = (uint64_t)(prod - 1);
            if (mod_pow(d, exponent, mod) == 1) {
                result = d * (seq_t)m;
                break;
            }
        }
    }
    a_cache[n] = result;
    return result;
}
