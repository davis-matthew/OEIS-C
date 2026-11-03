/*
 * A247760  Numbers in decimal representation, such that in Polish their
 *          digits are in alphabetic order.
 * https://oeis.org/A247760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 2497, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9783 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An algorithm that generates numbers whose decimal digits are in
   nondecreasing order according to the custom Polish alphabetical
   order of digit names. The sequence is enumerated in increasing
   numeric order by filtering each integer. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* mapping of digit -> its position in the Polish alphabetical order
   4 _ cztery, 2 _ dwa, 9 _ dziewięć, 1 _ jeden, 8 _ osiem,
   5 _ pięć, 6 _ sześć, 7 _ siedem, 3 _ trzy, 0 _ zero */
static const uint8_t DIGIT_ORDER[10] = {10,4,2,9,1,6,7,8,5,3};

/* check that the decimal digits of x are in non‑decreasing order
   according to DIGIT_ORDER */
static int is_sorted_custom(seq_t x)
{
    char buf[200];
    size_t len;
    seq_to_str(x, buf, &len);
    for (size_t i = 1; i < len; ++i) {
        uint8_t d1 = buf[i-1] - '0';
        uint8_t d2 = buf[i]   - '0';
        if (DIGIT_ORDER[d1] > DIGIT_ORDER[d2])
            return 0;
    }
    return 1;
}

/* cached sequence values */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t sz    = 0;
static seq_t cur_val = 0;

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cap = 128;
        cache = mem_new(seq_t, cap);
    }

    /* generate terms until we have at least n entries */
    while (sz < n) {
        while (!is_sorted_custom(cur_val))   /* skip invalid numbers */
            cur_val++;

        if (sz == cap) {                    /* grow the cache */
            cap <<= 1;
            cache = mem_resize(seq_t, cache, cap);
        }

        cache[sz++] = cur_val;              /* store the valid term */
        cur_val++;                           /* next candidate */
    }

    return cache[n-1];
}
