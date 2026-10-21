/*
 * A296762  Numbers whose base-20 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296762: numbers in base‑20 whose rises equal falls.
   Computed by brute‑force scanning. */
#include <stdlib.h>
#include <stdbool.h>

/* Check if a number has an equal number of rises and falls in base‑20. */
static bool balanced_rises_falls(seq_t x)
{
    /* Extract base‑20 digits into a buffer.  A 128‑bit number is
       at most 87 digits in base 20, so 128 is sufficient for all
       supported seq_t sizes. */
    uint8_t digits[128];
    idx_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 20);
        tmp /= 20;
    }
    if (len <= 1) return true;          /* single digit: 0 rises/0 falls */

    unsigned int rises = 0, falls = 0;
    for (idx_t i = 0; i + 1 < len; ++i) {
        uint8_t a = digits[i];
        uint8_t b = digits[i + 1];
        if (a < b) ++rises;
        else if (a > b) ++falls;
    }
    return rises == falls;
}

/* Cache of computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_candidate = 1;          /* next integer to test */

/* Return the nth term (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Lazy initialization of the cache. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_size = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        /* Grow cache capacity if necessary. */
        if (cache_size >= cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }

        /* Test the current candidate. */
        if (balanced_rises_falls((seq_t)next_candidate)) {
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
