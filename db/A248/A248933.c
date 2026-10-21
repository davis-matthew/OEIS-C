/*
 * A248933  Decimal expansion of 2^2281 - 1, the 17th Mersenne prime
 *          A000668(17).
 * https://oeis.org/A248933
 *
 * Offset:    687    (a(687) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = decimal digit (n‑687) of 2^2281 − 1 (for n ≥ 687)
 * Range:     u8 n <= 1373, u16 n <= 1373, u32 n <= 1373, u64 n <= 1373,
 *            u128 n <= 1373, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static char digits_str[700];
static size_t digits_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (n < 687) {               /* indices below offset are undefined */
        return (seq_t)0;
    }

    /* Compute the decimal representation of 2^2281 − 1 once. */
    if (!initialized) {
        unsigned char dec[700] = {0};
        size_t len = 1;
        dec[0] = 1;              /* start with 1 */

        /* Multiply by 2, 2281 times. */
        for (int k = 0; k < 2281; ++k) {
            unsigned int carry = 0;
            for (size_t i = 0; i < len; ++i) {
                unsigned int prod = (unsigned int)dec[i] * 2U + carry;
                dec[i] = prod % 10U;
                carry   = prod / 10U;
            }
            while (carry) {
                dec[len++] = carry % 10U;
                carry /= 10U;
            }
        }

        /* Subtract 1 to obtain 2^2281 − 1. */
        size_t i = 0;
        while (true) {
            if (dec[i] > 0U) {
                dec[i]--;
                break;
            } else {
                dec[i] = 9U;
                ++i;
            }
        }

        /* Store digits in a null‑terminated string (most significant first). */
        digits_len = len;
        for (size_t j = 0; j < len; ++j) {
            digits_str[digits_len - 1 - j] = (char)(dec[j] + '0');
        }
        digits_str[digits_len] = '\0';
        initialized = true;
    }

    size_t offset = (size_t)(n - 687U);

    if (offset >= digits_len) {
        return (seq_t)0;          /* out of range (should not happen) */
    }

    return (seq_t)(digits_str[offset] - '0');
}
