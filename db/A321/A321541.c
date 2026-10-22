/*
 * A321541  a(0)=1; thereafter a(n) = 3*a(n-1) with digits rearranged into
 *          nonincreasing order.
 * https://oeis.org/A321541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321541: a(0)=1; a(n)=3*a(n-1) with digits rearranged into nonincreasing order. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate a buffer large enough for the maximum number of digits. */
    size_t maxlen = (size_t)n + 5;
    uint8_t *digits = (uint8_t *)malloc(maxlen);
    if (!digits) {
        return (seq_t)0;   /* Out‑of‑memory: unlikely for typical inputs. */
    }

    /* Initialise a(0) = 1.  digits[0] holds the least‑significant digit. */
    size_t len = 1;
    digits[0] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        unsigned freq[10] = {0};
        unsigned carry = 0;

        /* Multiply the current number by 3 and count the resulting digits. */
        for (size_t i = 0; i < len; ++i) {
            unsigned prod = (unsigned)digits[i] * 3 + carry;
            uint8_t d = prod % 10;
            carry = prod / 10;
            ++freq[d];
        }
        while (carry) {
            uint8_t d = carry % 10;
            carry /= 10;
            ++freq[d];
        }

        /* Build the next number’s digits in ascending order (least‑significant first). */
        size_t idx = 0;
        for (uint8_t d = 0; d < 10; ++d) {
            for (unsigned c = 0; c < freq[d]; ++c) {
                digits[idx++] = d;
            }
        }
        len = idx;
    }

    /* Convert the ascending digits (least‑significant first) to a seq_t value. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * 10 + (seq_t)digits[len - 1 - i];
    }

    free(digits);
    return result;
}
