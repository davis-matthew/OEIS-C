/*
 * A370842  Numbers k that can be added without carries to their digit
 *          reversal (A004086(k)).
 * https://oeis.org/A370842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th nonnegative integer k such that k + rev(k) has no carries during decimal addition. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Test whether a number satisfies the “no‑carry” condition. */
static bool no_carry_exists(seq_t k)
{
    /* Store the decimal digits in little‑endian order. */
    uint8_t digits[200];
    size_t len = 0;
    seq_t tmp = k;

    if (tmp == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > (seq_t)0) {
            digits[len++] = (uint8_t)(tmp % (seq_t)10);
            tmp /= (seq_t)10;
        }
    }

    for (size_t i = 0; i < len / 2; ++i) {
        if (digits[i] + digits[len - 1 - i] > 9)
            return false;
    }
    if (len % 2 == 1 && (digits[len / 2] * 2) > 9)
        return false;

    return true;
}

/* Return the n‑th element of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Persistent cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;

    if (cache == NULL) {          /* first call: allocate initial cache */
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_len < n) {       /* produce terms until we have enough */
        seq_t cand = next_candidate;
        if (no_carry_exists(cand)) {
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                cache = tmp;
            }
            cache[cache_len++] = cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
