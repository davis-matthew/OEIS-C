/*
 * A345529  Primes that yield a prime when any single digit is replaced by its
 *          10's complement.
 * https://oeis.org/A345529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 24, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search for primes whose any single digit
 * replaced by its 10's complement remains prime.  The implementation
 * stores the previously found terms in a static cache and generates
 * further terms on demand. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache of found terms.  Elements never exceed seq_t. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t current_candidate = 3;   /* start from the smallest odd prime */

/* Return true if p is a prime with the required digit‑replacement property. */
static bool good_prime(seq_t p)
{
    /* Extract decimal digits, least significant first. */
    uint8_t digits[256];
    size_t len = 0;
    seq_t tmp = p;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    if (len == 0)
        return false;          /* should never happen for primes > 0 */

    /* Test each single‑digit replacement. */
    for (size_t pos = 0; pos < len; ++pos) {
        uint8_t orig = digits[pos];
        uint8_t newd = (orig > 0 ? 10 - orig : 0);

        seq_t y = 0;
        for (long long j = (long long)len - 1; j >= 0; --j) {
            uint8_t d = digits[(size_t)j];
            if ((size_t)j == pos)
                d = newd;
            y = y * 10 + d;
        }

        if (!is_prime_big(y))
            return false;
    }
    return true;
}

/* Generate terms up to index n. */
static void generate_up_to(idx_t n)
{
    size_t target = (size_t)n;
    if (cache_len >= target)
        return;

    while (cache_len < target) {
        /* Skip non‑prime candidates. */
        while (!is_prime_big(current_candidate)) {
            if (current_candidate <= 2)
                current_candidate = 3;
            else
                current_candidate += 2;
        }

        /* Test the property. */
        if (good_prime(current_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = current_candidate;
        }

        /* Advance to the next odd number. */
        if (current_candidate <= 2)
            current_candidate = 3;
        else
            current_candidate += 2;
    }
}

/* Public entry point – returns the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    generate_up_to(n);
    return cache[n - 1];
}
