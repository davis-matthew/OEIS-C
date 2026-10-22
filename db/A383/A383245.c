/*
 * A383245  Nonnegative integers that contain the digit 0, or an even digit d
 *          immediately followed by a digit >= d.
 * https://oeis.org/A383245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer that contains digit 0 or
 *            has an even digit d immediately followed by a digit >= d
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_valid_property
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_valid_property.h"

seq_t a(idx_t n)
{
    idx_t count = (idx_t)0;
    seq_t current = (seq_t)0;
    while (1) {
        if (has_valid_property(current)) {
            count += (idx_t)1;
            if (count == n) return current;
        }
        current += (seq_t)1;
    }
}
