/*
 * A318536  Integers a(n) such that [a(n) + k] is a palindrome in base 10,
 *          with k < 10.
 * https://oeis.org/A318536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 246, u16 n <= 7417, u32 n <= 9599, u64 n <= 9599,
 *            u128 n <= 9599, big any n
 * Helpers:   build_from_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer x such that the smallest decimal
   palindrome ≥ x differs from x by at most 9.  */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_from_digits.h"

/* Return the smallest decimal palindrome ≥ x.  */
static seq_t next_pal(seq_t x)
{
    unsigned char digits[128];   /* msb → lsb */
    size_t len = 0;

    /* Build the msb‑first digit array */
    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t t = x;
        while (t) {
            digits[len++] = (unsigned char)(t % 10);
            t = t / 10;
        }
        /* Reverse to msb→lsb */
        for (size_t i = 0; i < len / 2; ++i) {
            unsigned char tmp = digits[i];
            digits[i]        = digits[len - 1 - i];
            digits[len - 1 - i] = tmp;
        }
    }

    /* Construct palindrome by mirroring */
    unsigned char pal[128];
    memcpy(pal, digits, len);
    for (size_t i = 0, j = len - 1; i < j; ++i, --j)
        pal[j] = pal[i];

    seq_t cand = build_from_digits(pal, len);
    if (cand >= x)
        return cand;

    /* Increment the middle part and propagate carry leftwards */
    int idx = (int)((len % 2) ? (len / 2) : (len / 2 - 1));
    int carry = 1;
    for (int k = idx; k >= 0 && carry; --k) {
        int v = digits[k] + carry;
        if (v == 10) {
            digits[k] = 0;
            carry = 1;
        } else {
            digits[k] = (unsigned char)v;
            carry = 0;
        }
    }

    /* All digits were 9 → new length palindrome 10…01 */
    if (carry) {
        seq_t res = 1;
        for (size_t c = 0; c < len; ++c)
            res = res * 10;   /* 10^len */
        return res + 1;      /* 10^len + 1 */
    }

    /* Mirror again after the increment */
    memcpy(pal, digits, len);
    for (size_t i = 0, j = len - 1; i < j; ++i, --j)
        pal[j] = pal[i];

    return build_from_digits(pal, len);
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t cur = 1;   /* current number to test */

    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_size < n) {
        seq_t pal = next_pal(cur);
        seq_t diff = pal - cur;
        if (diff <= 9) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cur;
        }
        ++cur;
    }
    return cache[n - 1];
}
