/*
 * A386350  Primes without {5, 6} as digits.
 * https://oeis.org/A386350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime that does not contain digits 5 or 6
 * Range:     u8 n <= 44, u16 n <= 2619, u32 n <= 1720, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static seq_t *prime_cache = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;
static uint64_t next_candidate = 2;

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes in the cache */
    while (prime_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;

        /* Check that val contains neither digit 5 nor 6 */
        seq_t temp = val;
        bool ok = true;
        while (temp > 0) {
            seq_t d = temp % 10;
            if (d == 5 || d == 6) {
                ok = false;
                break;
            }
            temp = temp / 10;
        }

        /* If allowed, test for primality and store */
        if (ok && is_prime_big(val)) {
            append_candidate(&prime_cache, &prime_len, &prime_cap, val);
        }

        /* Advance to next candidate */
        ++next_candidate;
    }

    /* Return the (n-1)-th index (offset 1) */
    return prime_cache[(size_t)n - 1];
}
