/*
 * A359943  a(n) = Sum_{d|n, d-1 is cube} d.
 * https://oeis.org/A359943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d-1 is cube} d
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A359943. */
#include <stdint.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 */
    seq_t result = 0;
    idx_t max_m = int_cube_root(n - 1);   /* largest m such that m^3 <= n-1 */
    for (idx_t m = 0; m <= max_m; ++m) {
        seq_t cube = (seq_t)m * (seq_t)m * (seq_t)m;  /* m^3 */
        seq_t d = cube + 1;          /* divisor candidate */
        if ((uint64_t)n % (uint64_t)d == 0) {   /* d divides n ? */
            result += d;
        }
    }
    return result;
}
