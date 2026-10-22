/*
 * A380358  Numbers whose binary expansion ends with 11 and does not contain
 *          adjacent zeros.
 * https://oeis.org/A380358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a
 *   Compute the nth term of OEIS sequence A380358.
 *
 * Algorithm:
 *   Enumerate positive integers starting from 3.
 *   Keep only those whose binary representation ends with "11"
 *   and contains no occurrence of "00".
 *   Count the kept numbers until the desired index n is reached and
 *   return that value.
 */

#include <stdbool.h>

/* Helper: check whether a number satisfies the sequence condition.
 * It ends with binary '11' and has no adjacent zero bits.
 */
static inline bool good_value(seq_t val)
{
    /* Must end with binary '11' */
    if ((val & 3) != 3) {
        return false;
    }

    /* No two consecutive zeros anywhere in the binary representation */
    seq_t tmp = val;
    while (tmp > 1) {
        if ((tmp & 3) == 0) {   /* pattern 00 */
            return false;
        }
        tmp >>= 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t num = 3;   /* smallest number that can satisfy the condition */
    idx_t count = 0;

    for (;; ++num) {          /* iterate all integers >= 3 */
        if (good_value(num)) {
            ++count;
            if (count == n) {
                return num;
            }
        }
    }
}
