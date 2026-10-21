/*
 * A249953  Primes with distinct digits: a(n) is the least prime > a(n-1) such
 *          that a(n-1) and a(n) share just one digit.
 * https://oeis.org/A249953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 145, u32 n <= 163, u64 n <= 163,
 *            u128 n <= 163, big any n
 * Helpers:   get_digits, next_prime, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A249953
 * a(1) = 13
 * a(n) = least prime > a(n-1) that has all distinct digits and shares exactly
 *        one digit with a(n-1).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"
#include "../utils/next_prime.h"
#include "../utils/seq_alloc_new_array.h"

/* Memoisation buffer */
static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_size = 0;

/* Append a value to the cache, expanding if necessary */
static void cache_append(seq_t value)
{
    if (seq_cache_size >= seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 4;
        seq_t *new_buf = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < seq_cache_size; ++i)
            new_buf[i] = seq_cache[i];
        seq_cache = new_buf;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_size++] = value;
}

/* Ensure the cache contains at least n terms */
static void cache_ensure(idx_t n)
{
    if (seq_cache_cap == 0) {
        seq_cache_cap = (n > 4) ? n : 4;
        seq_cache = seq_alloc_new_array(seq_cache_cap);
    }

    while (seq_cache_size < (size_t)n) {
        /* First term */
        if (seq_cache_size == 0) {
            cache_append((seq_t)13);
            continue;
        }

        seq_t prev = seq_cache[seq_cache_size - 1];

        /* Digit presence for the previous term */
        bool prev_digits[10] = {false};
        uint8_t buf[32];
        size_t len;
        get_digits(prev, buf, &len);
        for (size_t i = 0; i < len; ++i)
            prev_digits[buf[i]] = true;

        /* Find next suitable prime */
        uint64_t cand = next_prime((uint64_t)prev);
        for (;;) {
            uint8_t cand_buf[32];
            size_t cand_len;
            get_digits((seq_t)cand, cand_buf, &cand_len);

            /* Check that cand has all distinct digits */
            bool seen[10] = {false};
            bool distinct = true;
            for (size_t i = 0; i < cand_len; ++i) {
                uint8_t d = cand_buf[i];
                if (seen[d]) { distinct = false; break; }
                seen[d] = true;
            }
            if (!distinct) { cand = next_prime(cand); continue; }

            /* Count common distinct digits */
            int common = 0;
            for (size_t i = 0; i < cand_len; ++i)
                if (prev_digits[cand_buf[i]])
                    ++common;

            if (common == 1)
                break;                  /* found the next term */

            cand = next_prime(cand);
        }

        cache_append((seq_t)cand);
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for non‑positive indices */
    cache_ensure(n);
    return seq_cache[n - 1];
}
