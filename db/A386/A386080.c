/*
 * A386080  Primes having only {0, 5, 7, 9} as digits.
 * https://oeis.org/A386080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 61, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates a(n) : the n‑th prime whose decimal representation
 * uses only the digits {0,5,7,9}.  The implementation is a lazy
 * generator that keeps a cache of found primes and expands it
 * as needed, using a simple digit counter in positional base–4
 * (0→0, 1→5, 2→7, 3→9).  Primality is tested with
 * isPrimeGeneric from ../utils/isPrimeGeneric.h. */
 
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/isPrimeGeneric.h"

/* Cache of primes found so far */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Current number in a base‑4 digit array (most‑significant first) */
static uint8_t digits[64];
static idx_t cur_len = 0;
static bool initialized = false;

/* Convert current digits to the corresponding decimal value */
static seq_t digits_to_value(void)
{
    seq_t val = 0;
    for (idx_t i = 0; i < cur_len; ++i) {
        seq_t d = (digits[i] == 0) ? 0
              : (digits[i] == 1) ? 5
              : (digits[i] == 2) ? 7
              : 9;                     /* digits[i] == 3 */
        val = val * 10 + d;
    }
    return val;
}

/* Advance the base‑4 digit array to the next admissible number */
static void increment_digits(void)
{
    if (!initialized) {        /* first call: 5 */
        cur_len = 1;
        digits[0] = 1;         /* maps to 5 */
        initialized = true;
        return;
    }

    int pos = (int)cur_len - 1; /* least‑significant digit */
    while (pos >= 0) {
        digits[pos] += 1;
        if (digits[pos] == 4) {
            if (pos == 0) { /* overflow of the leading digit */
                ++cur_len;
                digits[0] = 1; /* new leading 5 */
                for (idx_t i = 1; i < cur_len; ++i)
                    digits[i] = 0;
                break;
            } else {
                digits[pos] = 0;
                --pos;
                continue;
            }
        } else {
            break;
        }
    }
}

/* Ensure the cache can hold at least 'needed' elements.   */
static void ensure_cache(idx_t needed)
{
    if (cache_cap >= (size_t)needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real build you might handle the error differently. */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Append a prime to the cache */
static void append_cache(seq_t value)
{
    ensure_cache(cache_size + 1);
    cache[cache_size++] = value;
}

/* Public function: return a(n) for any n >= 1 (0 returns 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* behaviour for out of range is arbitrary */

    while (cache_size < (size_t)n) {
        seq_t candidate = digits_to_value();
        if (isPrimeGeneric(candidate))
            append_cache(candidate);
        increment_digits();
    }
    return cache[(size_t)n - 1];
}
