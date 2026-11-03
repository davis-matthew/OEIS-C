/*
 * A262582  Concatenation of the numbers from 1 to n but omitting 12.
 * https://oeis.org/A262582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the numbers 1,2,3,… skipping 12 ; that
 *            is, a(n) is the concatenation of the first n natural numbers
 *            after omitting the integer 12
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result    = 0;
    idx_t processed = 0;

    for (idx_t i = 1; processed < n; ++i) {
        if (i == 12) continue;                  /* skip the number 12   */
        idx_t d   = num_digits_idx(i);               /* number of decimal digits */
        seq_t pw  = pow10(d);                   /* 10^d                      */
        result    = result * pw + (seq_t)i;     /* append i                */
        ++processed;
    }
    return result;
}
