/*
 * A334511  Eventual period of a single cell in rule 9 cellular automaton in a
 *          cyclic universe of width n.
 * https://oeis.org/A334511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 - (-1)^n)*n/2 for n > 10; for n <= 10 the value is
 *            computed by a short simulation
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   simulate_period
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/simulate_period.h"

seq_t a(idx_t n) {
    if (n <= 10) {
        return simulate_period(n);
    } else {
        if ((n & 1ULL) == 0ULL) {
            return (seq_t)n;         /* even n: a(n) = n */
        } else {
            return ((seq_t)n) << 1; /* odd n: a(n) = 2n */
        }
    }
}
