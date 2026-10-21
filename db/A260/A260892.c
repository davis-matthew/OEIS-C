/*
 * A260892  Primes having only {1, 7, 8} as digits.
 * https://oeis.org/A260892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 4209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime whose decimal digits are only 1, 7, 8
   (OEIS sequence A260892).  The function builds a cache of primes
   found with those digits and grows it lazily until it has at least
   n entries. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

static const uint8_t DIGITS[3] = {1, 7, 8};
static const uint8_t LAST_D[2] = {1, 7};

static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;
static size_t last_gen_len = 0;

static void rec_generate(seq_t cur, size_t rem)
{
    if (rem == 0) {
        if (isPrimeGeneric(cur))
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap, cur);
        return;
    }
    const uint8_t *opts = (rem == 1) ? LAST_D : DIGITS;
    size_t nopt = (rem == 1) ? 2 : 3;
    for (size_t i = 0; i < nopt; ++i) {
        /* Prevent overflow for native unsigned types */
        if (sizeof(seq_t) <= 8) {
            seq_t digit = (seq_t)opts[i];
            seq_t maxv = ~((seq_t)0);
            if (cur > (maxv - digit) / 10) continue;
        }
        rec_generate(cur * 10 + opts[i], rem - 1);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0 */

    if (!prime_cache) {
        prime_cache_cap = 64;
        prime_cache = (seq_t *)malloc(prime_cache_cap * sizeof(seq_t));
        prime_cache_len = 0;
    }

    while (prime_cache_len < (size_t)n) {
        size_t len = last_gen_len + 1;
        rec_generate(0, len);
        last_gen_len = len;
    }

    return prime_cache[(size_t)n - 1];
}
