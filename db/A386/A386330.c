/*
 * A386330  Primes without {1, 4} as digits.
 * https://oeis.org/A386330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, has_forbidden_digits_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes whose decimal representation does not contain digits 1 or 4.
   The function generates these primes on demand and caches them between
   calls. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime.h"
#include "../utils/has_forbidden_digits_idx.h"

static seq_t *cached = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;
static idx_t next_candidate = 2;

static void append_prime(seq_t prime)
{
    if (cached_len == cached_cap)
        ensure_capacity(cached_len + 1, &cached_cap, (void **)&cached, sizeof(seq_t));
    cached[cached_len++] = prime;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (cached_len < n)
    {
        while (1)
        {
            if (has_forbidden_digits_idx(next_candidate))
            {
                ++next_candidate;
                continue;
            }

            if (is_prime(next_candidate))
            {
                append_prime((seq_t)next_candidate);
                ++next_candidate;
                break;
            }

            ++next_candidate;
        }
    }

    return cached[n - 1];
}
