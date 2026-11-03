/*
 * A295660  Binary weight of Euler phi: a(n) = A000120(A000010(n)).
 * https://oeis.org/A295660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000120(A000010(n))
 * Range:     u8 n <= 594, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9361 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset starts at 1 */

    /* Compute Euler's totient φ(n).  The static function `phi`
       provided by utils/phi.h gives the value directly as seq_t. */
    seq_t phi_val = totient(n);

    /* Count the binary weight (number of 1‑bits) of φ(n). */
    int bits = 0;
    while (phi_val != 0) {
        bits += (int)(phi_val & 1);
        phi_val >>= 1;
    }
    return (seq_t)bits;
}
