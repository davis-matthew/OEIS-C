/*
 * A323521  Numbers whose number of prime factors counted with multiplicity
 *          (A001222) is not a perfect square.
 * https://oeis.org/A323521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   bigomega, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323521: a(n) = the n‑th positive integer whose number of prime factors
   counted with multiplicity (bigOmega) is not a perfect square. */
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t cur = 1;
    idx_t count = 0;
    while (true) {
        seq_t omega = bigomega(cur);
        seq_t s = floor_sqrt(omega);
        if (s * s != omega) {
            count++;
            if (count == n) {
                return (seq_t)cur;
            }
        }
        cur++;
    }
}
