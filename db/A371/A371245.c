/*
 * A371245  Number of factorizations of n into factors > 4.
 * https://oeis.org/A371245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 4.
 *            The trivial factorization consisting of the single factor n
 *            itself is counted. This implementation uses a recursive helper
 *            with a non‑decreasing factor order to avoid duplicate
 *            factorizations
 * Range:     u8 n <= 9995, u16 n <= 9995, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   _factor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_factor_count.h"

seq_t a(idx_t n) {
    if (n == 1)          /* by convention a(1) = 1 */
        return 1;
    if (n < 5)           /* 2, 3, 4 have no factorizations into factors > 4 */
        return 0;
    return _factor_count(n, 5);  /* start with the smallest allowed factor */
}
