/*
 * A297152  Positive numbers whose decimal expansion contains a 3-digit
 *          substring abc such that b > a > c.
 * https://oeis.org/A297152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose decimal expansion *
 *            contains a three‑digit substring abc such that b > a > c
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   contains_valid_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_valid_triple.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num   = 0;

    while (true) {
        ++num;
        if (contains_valid_triple(num)) {
            ++count;
            if (count == n)
                return num;
        }
    }
}
