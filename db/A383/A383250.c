/*
 * A383250  Nonnegative integers not ending with the digit 1 and such that
 *          every odd digit except the rightmost is immediately followed by a
 *          smaller digit.
 * https://oeis.org/A383250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A383250 by incremental enumeration of
   nonnegative integers that do not end with 1 and for which every
   odd digit except the rightmost is followed by a smaller digit. */

#include <stdbool.h>

static bool is_valid(seq_t x)
{
    /* Reject numbers ending with 1 */
    if ((x % (seq_t)10) == (seq_t)1)
        return false;
    seq_t prev = x % (seq_t)10;            /* rightmost digit   */
    seq_t temp = x / (seq_t)10;
    while (temp > (seq_t)0) {
        seq_t curr = temp % (seq_t)10;
        /* If the current digit is odd, it must be greater than the digit to its right */
        if ((curr & (seq_t)1) == (seq_t)1) {
            if (curr <= prev)
                return false;
        }
        prev = curr;
        temp = temp / (seq_t)10;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t value = 0;
    while (count < n) {
        if (is_valid(value)) {
            ++count;
            if (count == n)
                return value;
        }
        ++value;
    }
    /* Should never be reached */
    return (seq_t)0;
}
