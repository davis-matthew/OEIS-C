/*
 * A292451  Numbers where 1 outnumbers any other digit.
 * https://oeis.org/A292451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   more_ones_than_others
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers where the count of digit 1 exceeds the count of any other digit */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/more_ones_than_others.h"

/* Return the nth term (1‑based index) of the sequence */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n=0, return 0 safely */
        return 0;

    seq_t candidate = 1;   /* start checking from 1 */
    idx_t found = 0;

    while (true) {
        if (more_ones_than_others(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
