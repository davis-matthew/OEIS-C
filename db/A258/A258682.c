/*
 * A258682  Remove from n^2 all digits of n from left to right, in decimal
 *          representation.
 * https://oeis.org/A258682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, get_digits, remove_leftmost
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = remove from n² all digits of n from left to right,
 *   taking the leftmost occurrence of each digit when removal occurs.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "../utils/digit_len.h"
#include "../utils/get_digits.h"
#include "../utils/remove_leftmost.h"

/* Reverse an array of uint8_t in place. */
static void reverse(uint8_t *arr, int len)
{
    for (int i = 0; i < len / 2; ++i) {
        uint8_t tmp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = tmp;
    }
}

/* Helper: fill buf with decimal digits of the index n (most significant first).
   The caller supplies a buffer large enough for the maximum number of digits
   of n (e.g., 25 bytes for a 64‑bit integer). */
static void idx_to_digits(idx_t n, uint8_t *buf, int *len)
{
    if (n == 0) {
        buf[0] = 0;
        *len = 1;
        return;
    }

    int pos = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        buf[pos++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* Reverse to most significant first */
    for (int i = 0; i < pos / 2; ++i) {
        uint8_t t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    *len = pos;
}

seq_t a(idx_t n)
{
    /* Step 1: compute n² */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* Step 2: get decimal digits of n² in MSB‑first order */
    int lenSq = digit_len(sq);
    uint8_t *sqDigits = (uint8_t *)malloc((size_t)lenSq * sizeof(uint8_t));
    if (!sqDigits) {
        /* Memory failure fallback: return 0 (acceptable for this sequence). */
        return 0;
    }
    size_t outLen = 0;
    get_digits(sq, sqDigits, &outLen);
    int len = (int)outLen;
    reverse(sqDigits, len);

    /* Step 3: get decimal digits of n (MSB‑first) */
    uint8_t nDigits[25];
    int nLen = 0;
    idx_to_digits(n, nDigits, &nLen);

    /* Step 4: remove leftmost occurrence of each digit of n from sqDigits */
    for (int i = 0; i < nLen; ++i) {
        remove_leftmost(sqDigits, &len, nDigits[i]);
    }

    /* Step 5: build the remaining number from the surviving digits */
    seq_t res = 0;
    for (int i = 0; i < len; ++i) {
        res = res * 10 + (seq_t)sqDigits[i];
    }

    free(sqDigits);
    return res;
}
