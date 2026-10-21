/*
 * A248035  Least positive integer m such that m + n divides phi(m)^2 +
 *          phi(n)^2, where phi(.) is Euler's totient function.
 * https://oeis.org/A248035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A248035:
 * a(n) = least positive integer m such that (m + n) | (φ(m)² + φ(n)²).
 * φ(k) denotes Euler's totient function.  The implementation relies on
 * the provided utility function `seq_t phi(idx_t m)`.
 * The value is computed by a simple search starting at m = 1; the
 * conjecture guarantees that a solution exists, and for n ≤ 10000
 * it is bounded by n² (except for the known exceptional case n = 33).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n = 0, handled gracefully */

    seq_t phi_n = phi(n);

    for (idx_t m = 1; ; ++m)
    {
        seq_t phi_m = phi(m);
        seq_t sum   = (seq_t)m + (seq_t)n;
        seq_t lhs   = phi_m * phi_m + phi_n * phi_n;

        if ((lhs % sum) == 0)
            return (seq_t)m;
    }
}
