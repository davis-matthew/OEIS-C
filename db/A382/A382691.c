/*
 * A382691  Alternating sum of the characteristic functions of k-th powers,
 *          with k >= 2: characteristic function of squares - c.f. of cubes +
 *          c.f. of 4th powers - ... .
 * https://oeis.org/A382691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k≥2} (-1)^k * χ_k(n), where χ_k(n)=1 if n is a perfect
 *            k‑th power, otherwise 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_perfect_kth_power, pow_check_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/is_perfect_kth_power.h"
#include "../utils/pow_check_uint64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0; /* definition in OEIS */

    seq_t sum = 0;
    /* Maximum exponent needed: 2^exp ≤ n => exp ≤ 64 for uint64_t n. */
    for (int k = 2; k <= 64; ++k) {
        if (is_perfect_kth_power(n, k)) {
            if (k % 2 == 0)
                sum += 1;
            else
                sum -= 1;
        }
    }
    return sum;
}
