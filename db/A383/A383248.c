/*
 * A383248  Nonnegative integers without the digit 9 such that every odd digit
 *          except the rightmost is immediately followed by a larger digit.
 * https://oeis.org/A383248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9088 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A383248
   Nonnegative integers without the digit 9 such that every odd digit
   except the rightmost is immediately followed by a larger digit.
   The values are returned in ascending order (a(1)=0). */

#include <stdbool.h>

/* Check whether x satisfies the property */
static bool valid_seq(seq_t x)
{
    seq_t prev = (seq_t)-1;   /* value of the digit to the right (none yet) */
    bool hasPrev = false;     /* true after we have processed at least one digit */

    while (x > 0) {
        seq_t d = x % 10;          /* current (less‑significant) digit */

        if (d == 9)                /* digit 9 is forbidden */
            return false;

        if (hasPrev) {             /* not the rightmost digit */
            /* If d is odd and its right neighbour is not larger */
            if ((d & 1) && prev <= d)
                return false;
        }

        hasPrev = true;
        prev    = d;
        x /= 10;
    }
    return true;                   /* 0 is accepted */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t val   = 0;

    for (;;) {
        if (valid_seq((seq_t)val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
        ++val;
    }
}
