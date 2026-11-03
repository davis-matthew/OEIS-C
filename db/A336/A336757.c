/*
 * A336757  Number of primitive integer-sided triangles whose sides a < b < c
 *          are in arithmetic progression with a perimeter = n.
 * https://oeis.org/A336757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n%3 != 0 or floor(n/3) < 3; otherwise a(n) =
 *            phi(n/3)/2
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided calc_phi helper to compute
   Euler's totient function phi.  The result is halved as required
   for primitive arithmetic progression triangles. */

#include <stdint.h>
#include "../utils/totient_seq.h"

seq_t a(idx_t n) {
    if (n % 3 != 0) return 0;           /* not divisible by 3 → 0 */
    idx_t b = n / 3;                    /* middle side length */
    if (b < 3) return 0;                 /* b must be at least 3 */

    seq_t phi = totient_seq((seq_t)b);      /* phi(b) */
    return phi / 2;                     /* phi(b) is even for b >= 3 */
}
