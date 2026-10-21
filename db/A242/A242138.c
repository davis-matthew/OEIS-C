/*
 * A242138  Binary representation of periodic binary numbers, ordered by their
 *          decimal values.
 * https://oeis.org/A242138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   qualifies_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: binary representation of periodic binary numbers, ordered by decimal values.
   Compute by enumerating decimal numbers made of 0's and 1's and
   testing whether the string of digits is periodic when padded with leading zeros. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/qualifies_digits.h"

/* Cache of found terms for memoisation.  Cache grows lazily. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Generator state: current length of decimal strings (len >= 1)
   and current suffix value for that length. */
static unsigned gen_len = 1;
static uint64_t gen_suffix = 0;

/* Helper to grow the cache. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failed – keep old cache */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; neutral value */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* build the next candidate number from current gen_len and gen_suffix */
        unsigned len = gen_len;
        uint8_t digits[64];
        if (len > 0) {
            digits[0] = 1; /* leading digit is always 1 (no leading zeros in decimal) */
            for (unsigned i = 1; i < len; ++i) {
                /* the suffix bits are represented by gen_suffix in binary order */
                unsigned shift = (len - 1 - i);
                digits[i] = (gen_suffix >> shift) & 1;
            }
        }

        /* compute the decimal value of this candidate */
        seq_t val = 1;
        for (unsigned i = 1; i < len; ++i) {
            val = val * 10 + digits[i];
        }

        /* test the periodicity condition */
        if (qualifies_digits(digits, len)) {
            if (cache_len >= cache_cap) grow_cache();
            cache[cache_len++] = val;
        }

        /* advance generator */
        ++gen_suffix;
        if (len > 0 && gen_suffix == ((uint64_t)1 << (len - 1))) {
            gen_suffix = 0;
            ++gen_len;
        }
    }

    return cache[(size_t)n - 1];
}
