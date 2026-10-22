/*
 * A330286  Tally the digits of n: list the distinct digits of n in order of
 *          appearance, each followed by the respective count, concatenate.
 * https://oeis.org/A330286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   For n>0, a(n) = concatenation of each distinct digit of n (in
 *            order of appearance) followed by its count; a(0)=1
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Digit array: maximum 20 digits for a 64‑bit unsigned integer. */
    int digits[20];
    size_t len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = (int)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    unsigned int count[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        count[digits[i]]++;
    }

    seq_t result = 0;
    int seen[10] = {0};

    while (len--) {
        int d = digits[len];
        if (!seen[d]) {
            /* append the digit itself */
            result = result * (seq_t)10 + (seq_t)d;

            /* append its count (one or two decimal digits). */
            unsigned int c = count[d];
            if (c >= 10) {
                result = result * (seq_t)100 + (seq_t)c;
            } else {
                result = result * (seq_t)10 + (seq_t)c;
            }
            seen[d] = 1;
        }
    }
    return result;
}
