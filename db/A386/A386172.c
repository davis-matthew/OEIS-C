/*
 * A386172  Primes having only {3, 4, 5, 9} as digits.
 * https://oeis.org/A386172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 142, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal representation uses only digits 3,4,5,9 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static const uint8_t allowed_digits[4] = {3, 4, 5, 9};

/* powers of four, up to the first overflow (4^19 < 2^63 < 4^20) */
static size_t pow4[32];
static bool pow4_inited = false;

static void init_pow4(void)
{
    if (pow4_inited) return;
    pow4[0] = 1;
    for (size_t i = 1; i < 32; ++i) {
        if (pow4[i - 1] > (SIZE_MAX / 4)) {
            pow4[i] = 0;          /* overflow marker (never used) */
            break;
        }
        pow4[i] = pow4[i - 1] * 4;
    }
    pow4_inited = true;
}

/* dynamic array for cached primes */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

static void ensure_prime_cache(size_t need)
{
    if (need <= prime_cache_cap) return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

/* state of incremental generation */
static size_t current_length = 1; /* number of digits currently being processed */
static size_t idx_in_length = 0;  /* next candidate index within current_length */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, guard */

    size_t target = (size_t)n;
    if ((size_t)n <= prime_cache_len) {
        return prime_cache[(size_t)n - 1];
    }

    init_pow4();
    ensure_prime_cache(target);

    while (prime_cache_len < target) {
        size_t total = pow4[current_length];
        if (total == 0) {          /* overflow – never should happen */
            break;
        }

        if (idx_in_length >= total) { /* finished this length, move on */
            current_length++;
            idx_in_length = 0;
            continue;
        }

        /* Build the candidate number of current_length digits */
        seq_t val = 0;
        size_t idx = idx_in_length;
        for (size_t pos = 0; pos < current_length; ++pos) {
            size_t digit_index = (idx / pow4[current_length - 1 - pos]) % 4;
            val = val * (seq_t)10 + (seq_t)allowed_digits[digit_index];
        }

        if (is_prime_big(val)) {
            prime_cache[prime_cache_len++] = val;
            ++idx_in_length;           /* advance past this candidate */
            if (prime_cache_len == target) {
                break;
            }
            continue;
        }

        ++idx_in_length;               /* move to next candidate */
    }

    return prime_cache[(size_t)n - 1];
}
