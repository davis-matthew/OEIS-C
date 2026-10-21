/*
 * A283775  Numbers k such that floor(k*sqrt(3)) is even.
 * https://oeis.org/A283775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 9154, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283775: numbers k such that floor(k*sqrt(3)) is even.
   Computation: iterate k, test parity of floor(k*sqrt(3)) by integer
   arithmetic: floor(k*sqrt(3)) == floor(sqrt(3*k*k)).  Count until the
   nth such k is found.  Uses only the provided floor_sqrt routine. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t val = floor_sqrt((seq_t)3 * (seq_t)k * (seq_t)k);
        if ((val & 1) == 0) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
