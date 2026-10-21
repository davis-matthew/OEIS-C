/*
 * A265571  Smallest base-8 palindrome m >= n, written in base 8.
 * https://oeis.org/A265571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base-8 palindrome m >= n, written in base-8
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   digits_ge, to_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digits_ge.h"
#include "../utils/to_base8.h"

/* find next base-8 palindrome >= digits (original n digits) */
static void next_palindrome(const idx_t *orig, idx_t len, idx_t *pal, idx_t *pallen) {
    /* copy original digits */
    for (idx_t i = 0; i < len; ++i)
        pal[i] = orig[i];

    /* mirror left to right */
    for (idx_t i = 0; i < len / 2; ++i)
        pal[len - 1 - i] = pal[i];

    if (digits_ge(pal, orig, len)) {
        *pallen = len;
        return;
    }

    /* need to increment center */
    idx_t carry = 1;
    if (len & 1) { /* odd length */
        idx_t mid = len / 2;
        pal[mid] += carry;
        if (pal[mid] < 8) carry = 0;
        else { pal[mid] = 0; carry = 1; }

        for (idx_t i = mid; i > 0 && carry; --i) {
            pal[i - 1] += carry;
            if (pal[i - 1] < 8) carry = 0;
            else { pal[i - 1] = 0; carry = 1; }
        }
    } else { /* even length */
        idx_t left = len / 2 - 1;
        pal[left] += carry;
        if (pal[left] < 8) carry = 0;
        else { pal[left] = 0; carry = 1; }

        for (idx_t i = left; i > 0 && carry; --i) {
            pal[i - 1] += carry;
            if (pal[i - 1] < 8) carry = 0;
            else { pal[i - 1] = 0; carry = 1; }
        }
    }

    if (carry) {
        /* increase length: 1 0 ... 0 1 */
        *pallen = len + 1;
        pal[0] = 1;
        for (idx_t i = 1; i < *pallen - 1; ++i)
            pal[i] = 0;
        pal[*pallen - 1] = 1;
    } else {
        /* mirror again to enforce palindrome after increment */
        for (idx_t i = 0; i < len / 2; ++i)
            pal[len - 1 - i] = pal[i];
        *pallen = len;
    }
}

seq_t a(idx_t n) {
    idx_t digits[64];
    idx_t len;
    to_base8(n, digits, &len);

    /* find palindrome digits */
    idx_t pal[64];
    idx_t pallen;
    next_palindrome(digits, len, pal, &pallen);

    /* convert palindrome digits to decimal integer (as per OEIS representation) */
    seq_t res = 0;
    for (idx_t i = 0; i < pallen; ++i)
        res = res * 10 + pal[i];
    return res;
}
