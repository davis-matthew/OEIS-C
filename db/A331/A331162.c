/*
 * A331162  a(n) is the number of digits in the concatenation of a(0) to
 *          a(n-1) that are equal to the corresponding digit in the
 *          concatenation of all integers >= 0, with a(0) = 0.
 * https://oeis.org/A331162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   seqt_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   for n > 0, a(n) = a(n‑1) + (number of digits in a(n‑1)
   that equal the corresponding next digits of G,
   where G = 0,1,2,3,4,5,6,7,8,9,10,11,12,… */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seqt_digits.h"

/* Memoised sequence values */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;
static idx_t max_index = (idx_t)-1;

/* G‑state */
static uint64_t g_current = 0;
static unsigned char g_buf[32];
static size_t g_buf_len = 0;
static size_t g_buf_pos = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    a_cache = tmp;
    cache_cap = new_cap;
}

/* Refill g_buf with the decimal representation of g_current. */
static void refill_g_buffer(void)
{
    if (g_current == 0) {
        g_buf_len = 1;
        g_buf[0] = '0';
    } else {
        uint64_t t = g_current;
        unsigned char rev[32];
        size_t revlen = 0;
        while (t > 0) {
            rev[revlen++] = '0' + (t % 10);
            t /= 10;
        }
        g_buf_len = revlen;
        for (size_t i = 0; i < revlen; ++i)
            g_buf[i] = rev[revlen - 1 - i];
    }
    g_buf_pos = 0;
}

/* Return the next digit of G as an integer 0..9, consuming it. */
static int next_g_digit(void)
{
    if (g_buf_pos >= g_buf_len) {
        ++g_current;
        refill_g_buffer();
    }
    return g_buf[g_buf_pos++] - '0';
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Initialise on first call. */
    if (a_cache == NULL) {
        ensure_cache(1);
        a_cache[0] = (seq_t)0;
        max_index = 0;
        g_current = 0;
        refill_g_buffer(); /* G starts with '0' */
    }

    if (n > max_index) {
        ensure_cache(n);
        unsigned char dbuf[32];
        for (idx_t i = max_index + 1; i <= n; ++i) {
            seq_t prev = a_cache[i - 1];
            size_t len = seqt_digits(prev, dbuf);
            seq_t inc = 0;
            for (size_t j = 0; j < len; ++j) {
                int gd = next_g_digit();
                if (dbuf[j] == gd) inc++;
            }
            a_cache[i] = a_cache[i - 1] + inc;
        }
        max_index = n;
    }

    return a_cache[n];
}
