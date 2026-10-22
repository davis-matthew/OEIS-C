/*
 * A303255  "Wondrous representation" [left to right] of positive integer n, n
 *          >= 2.
 * https://oeis.org/A303255
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the wondrous representation by following the Collatz
 * trajectory of n.  The digits are recorded for each step
 * (1 for an odd step, 2 for an even step) while moving
 * from n down to 1, then the digits are output in reverse
 * order to form the decimal number a(n).  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be at least 2; for n == 1 the representation
     * is empty but the routine is not required to handle it. */
    seq_t x = (seq_t)n;

    /* Dynamically growable array of digits (1 or 2). */
    size_t cap = 128;
    size_t len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(*digits));
    if (!digits)
        return 0; /* Allocation failure – return zero as a sentinel. */

    /* Forward Collatz from n to 1, collecting digits. */
    while (x != (seq_t)1) {
        if ((x % (seq_t)2) == 0) {          /* even step – digit 2 */
            if (len == cap) {               /* grow when full */
                cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(*digits));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = 2;
            x /= (seq_t)2;
        } else {                            /* odd step – digit 1 */
            if (len == cap) {
                cap *= 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(*digits));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = 1;
            x = x * (seq_t)3 + (seq_t)1;
        }
    }

    /* Reverse the digit array to form the representation. */
    seq_t result = (seq_t)0;
    for (size_t i = len; i-- > 0; ) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
