/*
 * A385702  Numbers such that when the leftmost digit is moved to the unit's
 *          place the result is divisible by 7.
 * https://oeis.org/A385702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 9363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385702: numbers such that moving the leftmost decimal digit to
   the unit's place yields a number divisible by 7.  Computed by brute‑force
   search. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 0;

/* Ensure the static cache has room for at least needed elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Return true if x satisfies the defining property of the sequence. */
static bool good(seq_t x)
{
    int len = digit_len(x);                          /* number of decimal digits */
    seq_t pow10k = pow10((idx_t)(len - 1));          /* 10^(len-1) */
    seq_t d = x / pow10k;                           /* leftmost digit   */
    seq_t r = x % pow10k;                           /* remainder        */
    seq_t r_mod = r % (seq_t)7;                     /* r (mod 7) */
    seq_t d_mod = d % (seq_t)7;                     /* d (mod 7) */
    seq_t y_mod = (r_mod * (seq_t)3 + d_mod) % (seq_t)7;
    return y_mod == 0;
}

/* External (non‑static) function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Generate consecutive terms until we have n entries. */
    while (cache_len < n) {
        if (good(next_candidate)) {
            ensure_capacity(cache_len + 1);
            seq_cache[cache_len++] = next_candidate;
        }
        next_candidate = next_candidate + (seq_t)1;
    }
    return seq_cache[n - 1];
}
