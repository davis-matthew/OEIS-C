/*
 * A306494  Smallest number m such that n*3^m has 2 or more identical adjacent
 *          decimal digits.
 * https://oeis.org/A306494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest m such that n*3^m has at least one pair of equal adjacent
   decimal digits (OEIS A306494).  The function emulates the definition
   by brute‑forcing m from 0 to 35 and testing the decimal expansion of
   n·3^m for adjacent identical digits.  The helper get_digits extracts
   the decimal digits into a buffer; no floating point or string conversions
   are required.  This implementation works for any seq_t type provided
   by the framework, including big integers.  Complexity: O(35·D) per
   call, where D is the number of decimal digits of n·3^m (≤≈22 for the
   worst case m=35 and n<10^9).  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"   /* provides: void get_digits(seq_t, uint8_t *buf, size_t *len); */

/* Check whether a decimal number has two equal adjacent digits */
static bool has_adjacent_identical_digits(seq_t v)
{
    /* Maximum number of decimal digits for n*3^35 (n<10^9) is < 22.
       Allocate a buffer that is comfortably large for any seq_t. */
    uint8_t digits[128];
    size_t len = 0;

    /* get_digits fills 'digits' with the decimal digits of v
       and sets 'len' to the number of digits written. */
    get_digits(v, digits, &len);

    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == digits[i - 1]) {
            return true;
        }
    }
    return false;
}

/* Return a(n) for the sequence A306494 */
seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n;          /* current value of n*3^m */

    for (int m = 0; m <= 35; ++m) {
        if (has_adjacent_identical_digits(prod)) {
            return (seq_t)m;
        }
        /* multiply by 3 for the next power */
        prod = prod * (seq_t)3;
    }

    /* By definition a(n) <= 35 for all n>0, so this line is not
       expected to be reached.  Return a sentinel value. */
    return (seq_t)36;
}
