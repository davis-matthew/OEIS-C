/*
 * A366212  Numbers whose median of the digits is equal to 6.
 * https://oeis.org/A366212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   single-pass search for numbers whose sorted-digit median equals
 *            6
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_len, get_digits, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/get_digits.h"
#include "../utils/digit_len.h"

static seq_t *cache = NULL;
static size_t capacity = 0;
static size_t count = 0;
static seq_t last_checked = 0;

static bool median6(seq_t x)
{
    /* x must be positive */
    int len = digit_len(x);
    if (len <= 0) return false;
    uint8_t digits[64];
    size_t dlen;
    get_digits(x, digits, &dlen);          /* dlen == len */
    int cnt[10] = {0};
    for (size_t i = 0; i < dlen; ++i) cnt[digits[i]]++;

    int acc = 0;
    if (len & 1) {                         /* odd length: middle digit */
        int target = len / 2 + 1;
        for (int d = 0; d < 10; ++d) {
            acc += cnt[d];
            if (acc >= target) return d == 6;
        }
    } else {                               /* even length: average of two middle */
        int target1 = len / 2;
        int target2 = target1 + 1;
        int d1 = -1, d2 = -1;
        for (int d = 0; d < 10; ++d) {
            acc += cnt[d];
            if (d1 < 0 && acc >= target1) d1 = d;
            if (d2 < 0 && acc >= target2) d2 = d;
            if (d1 >= 0 && d2 >= 0) break;
        }
        return (d1 + d2) == 12;            /* (digit1+digit2)/2 == 6 */
    }
    return false;
}

seq_t a(idx_t n)
{
    /* @param n >= 1 */
    /* Ensure we have at least n terms computed */
    while (count < (size_t)n) {
        seq_t cand = last_checked + 1;
        if (median6(cand)) {
            if (count == capacity) {
                size_t newCap = capacity ? capacity * 2 : 64;
                cache = seq_realloc(cache, capacity, newCap);
                capacity = newCap;
            }
            cache[count++] = cand;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
