/*
 * A368086  The successive sums a(n) + a(n+1) reproduce the decimal expansion
 *          of the Champernowne constant.
 * https://oeis.org/A368086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n)+a(n+1) equals the nth digit of Champernowne constant.
 *            Recurrence: a(1) = 0; a(n+1) = digit(n) - a(n)
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;          /* 1‑based a(n) values, a_cache[1] = 0 */
static idx_t  a_cache_len = 0;
static idx_t  a_cache_cap = 0;

static uint8_t *digit_cache = NULL;    /* digits of Champernowne integer part */
static idx_t   digit_len = 0;
static idx_t   digit_cap = 0;
static uint64_t next_num_for_digits = 1;

/* Generate Champernowne digits until we have at least 'needed' digits */
static void ensure_digits(idx_t needed)
{
    if (needed <= digit_len) return;

    if (digit_cap == 0) digit_cap = 8;
    while (digit_cap < needed) digit_cap <<= 1;
    digit_cache = (uint8_t*)realloc(digit_cache, digit_cap * sizeof(uint8_t));

    while (digit_len < needed) {
        uint64_t num = next_num_for_digits;
        uint8_t temp[20];
        int t = 0;
        if (num == 0) temp[t++] = 0;
        while (num > 0) {
            temp[t++] = (uint8_t)(num % 10);
            num /= 10;
        }
        /* Append digits in correct order (most significant first) */
        for (int i = t - 1; i >= 0; --i) {
            if (digit_len >= digit_cap) {
                digit_cap <<= 1;
                digit_cache = (uint8_t*)realloc(digit_cache, digit_cap * sizeof(uint8_t));
            }
            digit_cache[digit_len++] = temp[i];
        }
        ++next_num_for_digits;
    }
}

/* Extend the a‑sequence cache up to index n */
static void ensure_a_cache(idx_t n)
{
    if (n <= a_cache_len) return;

    idx_t required = n + 1;            /* array is 1‑based; ignore index 0 */
    if (a_cache_cap == 0) a_cache_cap = 8;
    while (a_cache_cap < required) a_cache_cap <<= 1;
    a_cache = (seq_t*)realloc(a_cache, (a_cache_cap + 1) * sizeof(seq_t));

    if (a_cache_len == 0) {            /* first initialization */
        a_cache[1] = 0;
        a_cache_len = 1;
    }

    ensure_digits(n - 1);              /* digits needed for new terms */

    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        seq_t d = (seq_t)digit_cache[i - 2]; /* d = digit(n-1) */
        a_cache[i] = d - a_cache[i - 1];
    }
    a_cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined, but safe */
    ensure_a_cache(n);
    return a_cache[n];
}
