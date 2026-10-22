/*
 * A387904  Numbers in which there exist two, distinct, non-overlapping prime
 *          substrings that can be swapped to form a prime number.
 * https://oeis.org/A387904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_capacity, get_digits, is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A387904 by enumerating integers until the
   requested count is reached.  For each candidate number the
   function looks for two distinct, non‑overlapping prime
   substrings (without leading zeros) whose exchange produces a
   prime.  The algorithm stores the terms in a static cache. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

static inline bool is_prime_value(seq_t n)
{
    if (n <= (seq_t)1000000000ULL)
        return is_prime_small((uint64_t)n);
    return is_prime_big(n);
}

static bool has_swappable_prime_substrings(seq_t x)
{
    /* Extract digits (least significant first) */
    uint8_t d[64];
    size_t len;
    get_digits(x, d, &len);
    if (len == 0)
        return false;

    /* Reverse to most significant first */
    uint8_t rev[64];
    for (size_t i = 0; i < len; ++i)
        rev[i] = d[len - 1 - i];

    for (size_t i1 = 0; i1 < len; ++i1) {
        if (rev[i1] == 0)            /* leading zero not allowed */
            continue;
        seq_t val1 = 0;
        for (size_t j1 = i1; j1 < len; ++j1) {
            val1 = val1 * 10 + rev[j1];
            if (!is_prime_value(val1))
                continue;
            for (size_t i2 = j1 + 1; i2 < len; ++i2) {
                if (rev[i2] == 0)
                    continue;
                seq_t val2 = 0;
                for (size_t j2 = i2; j2 < len; ++j2) {
                    val2 = val2 * 10 + rev[j2];
                    if (!is_prime_value(val2))
                        continue;
                    if (val1 == val2)
                        continue;     /* substrings must differ */

                    /* Build the number after swapping the substrings */
                    uint8_t sw[64];
                    size_t p = 0;
                    for (size_t k = 0; k < i1; ++k)           /* prefix */
                        sw[p++] = rev[k];
                    for (size_t k = i2; k <= j2; ++k)         /* second part */
                        sw[p++] = rev[k];
                    for (size_t k = j1 + 1; k < i2; ++k)       /* middle part */
                        sw[p++] = rev[k];
                    for (size_t k = i1; k <= j1; ++k)         /* first part */
                        sw[p++] = rev[k];
                    for (size_t k = j2 + 1; k < len; ++k)     /* suffix */
                        sw[p++] = rev[k];

                    if (p != len)
                        continue; /* sanity check */

                    seq_t swapped = 0;
                    for (size_t k = 0; k < len; ++k)
                        swapped = swapped * 10 + sw[k];

                    if (swapped == x)
                        continue; /* swap must create a different number */
                    if (is_prime_value(swapped))
                        return true;
                }
            }
        }
    }
    return false;
}

static seq_t *cached_terms = NULL;  /* array of discovered terms */
static size_t cached_len   = 0;      /* number of terms stored   */
static size_t cached_cap   = 0;      /* capacity of the array    */
static uint64_t next_candidate = 0;   /* next integer to test     */

seq_t a(idx_t n)
{
    /* Grow the cache until we have at least n terms. */
    while (cached_len < (size_t)n) {
        ++next_candidate;
        if (has_swappable_prime_substrings(next_candidate)) {
            if (cached_len + 1 > cached_cap)
                ensure_capacity(cached_len + 1, &cached_cap,
                                (void **)&cached_terms, sizeof(seq_t));
            cached_terms[cached_len++] = (seq_t)next_candidate;
        }
    }
    return cached_terms[(size_t)n - 1];
}
