/*
 * A375348  a(n) is the mode of the digits of n! not counting trailing zeros
 *          (using -1 if multimodal).
 * https://oeis.org/A375348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 6000, i16 n <= 6000, i32 n <= 6000, i64 n <= 6000,
 *            i128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   a(n) = mode of the decimal digits of n! after removing all trailing zeros;
   if there is a tie for the maximum frequency, return -1.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* allocate an initial buffer for the digits of n! (least‑significant first) */
    size_t cap  = (n > 1 ? (size_t)n * 6 : 1);
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return (seq_t)-1;      /* allocation failure */

    size_t len = 1;           /* number of digits stored */
    digits[0] = 1;            /* 0! = 1 */

    /* build the factorial n! by multiplying by 2…n, digit by digit */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * (uint64_t)i + carry;
            digits[j] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry) {
            if (len >= cap) {
                cap <<= 1;                     /* double the capacity */
                uint8_t *tmp = (uint8_t *)realloc(digits, cap);
                if (!tmp) { free(digits); return (seq_t)-1; }
                digits = tmp;
            }
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    /* remove trailing zeros (least‑significant digits that are zero) */
    size_t start = 0;
    while (start < len && digits[start] == 0) ++start;

    /* count frequencies of the remaining digits */
    unsigned freq[10] = {0};
    for (size_t j = start; j < len; ++j) ++freq[digits[j]];

    /* detect the unique mode */
    int maxcnt   = 0;
    int modes    = 0;
    int mode_d   = -1;
    for (int d = 0; d < 10; ++d) {
        if ((int)freq[d] > maxcnt) {
            maxcnt = (int)freq[d];
            modes  = 1;
            mode_d = d;
        } else if ((int)freq[d] == maxcnt) {
            ++modes;
        }
    }

    free(digits);
    return (modes == 1) ? (seq_t)mode_d : (seq_t)-1;
}
