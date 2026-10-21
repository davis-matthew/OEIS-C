/*
 * A297290  Numbers whose base-16 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   satisfies_base16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297290: numbers whose base‑16 digits have greater up‑variation than down‑variation.
   The routine simply scans natural numbers in increasing order and counts those
   that satisfy the condition. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/satisfies_base16.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the offset 1, but avoid UB */
        return 0;

    idx_t found = 0;
    seq_t candidate = 0;

    while (found < n) {
        candidate = (seq_t)(candidate + 1);
        if (satisfies_base16(candidate)) {
            ++found;
        }
    }
    return candidate;
}
