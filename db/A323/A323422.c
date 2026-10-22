/*
 * A323422  Multiples of three whose sum of digits is not divisible by 3 until
 *          the final digit.
 * https://oeis.org/A323422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 5146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of OEIS sequence A323422:
 * "Multiples of three whose sum of digits is not divisible by 3 until
 * the final digit"
 *
 * The algorithm simply searches integers in increasing order and
 * selects those that satisfy the definition.  For each candidate the
 * digits are processed from most‑significant to least‑significant and
 * the running sum modulo 3 is inspected.  No intermediate value exceeds
 * its natural size, so arbitrary‑precision seq_t arithmetic is fully
 * respected.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x satisfies the property:  sum of digits is 0 mod 3
 * and no proper prefix sum is 0 mod 3.  Zero itself is accepted. */
static bool valid(seq_t x)
{
    /* Extract digits into a small array (not more than 200 digits is
       sufficient for the required ranges). */
    uint8_t digits[200];
    int len = 0;

    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t temp = x;
        while (temp > 0 && len < 200) {
            digits[len++] = (uint8_t)(temp % 10);
            temp /= 10;
        }
        /* If number has more than 200 digits we simply reject it;
           such cases do not arise for the expected input ranges. */
        if (len == 200 && temp > 0)
            return false;
    }

    /* Scan from most significant to least significant digit. */
    seq_t sum = 0;
    for (int i = len - 1; i >= 0; --i) {
        sum += digits[i];
        /* If this is a proper prefix (i > 0) and sum % 3 == 0,
           then the number is invalid. */
        if (i > 0 && (sum % 3) == 0)
            return false;
    }
    /* After full sum we must have remainder 0. */
    return (sum % 3) == 0;
}

/* The required sequence generation function. */
seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1)=0 is the first term. */
    seq_t count = 0;
    seq_t x = 0;

    for (;; ++x) {
        if (valid(x)) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
