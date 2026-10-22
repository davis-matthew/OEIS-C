/*
 * A329560  BII-numbers of antichains of sets with empty intersection.
 * https://oeis.org/A329560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth BII-number whose set‑system is an antichain with
 *            empty intersection
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_valid_bii
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_valid_bii.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, though n should be >= 1 */
    idx_t count = 0;
    for (idx_t val = 0; ; ++val) {
        if (is_valid_bii(val)) {
            ++count;
            if (count == n) {
                return (seq_t)val;
            }
        }
    }
}
