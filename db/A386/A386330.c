/*
 * A386330  Primes without {1, 4} as digits.
 * https://oeis.org/A386330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8847 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes whose decimal representation does not contain digits 1 or 4.
   The function generates these primes on demand and caches them between
   calls. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

static bool has_forbidden_digits_idx(idx_t x)
{
    while (x)
    {
        int d = (int)(x % 10);
        if (d == 1 || d == 4)
            return true;
        x /= 10;
    }
    return false;
}

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
