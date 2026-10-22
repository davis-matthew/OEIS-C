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
 * Helpers:   valid_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A383248
   Nonnegative integers without the digit 9 such that every odd digit
   except the rightmost is immediately followed by a larger digit.
   The values are returned in ascending order (a(1)=0). */

#include <stdbool.h>
#include "../utils/valid_seq.h"

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
