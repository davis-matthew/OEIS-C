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
 * Validated: standard, pass, 101 terms through n = 9607 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline uint64_t pow_check_uint64(uint64_t base, int exp, uint64_t limit)
{
    __int128 result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
        if (result > limit) return limit + 1; /* overflow or too big */
    }
    return (uint64_t)result;
}

/* Return 1 if n is a perfect k‑th power, else 0. */
static inline int is_perfect_kth_power(uint64_t n, int k)
{
    /* Special case n==1 handled outside of the loop. */
    uint64_t r = (uint64_t)(pow((double)n, 1.0 / k) + 0.5);
    if (r < 1) r = 1;

    /* Adjust down if we overshoot. */
    while (pow_check_uint64(r, k, n) > n) --r;
    /* Adjust up if we undershoot. */
    while (pow_check_uint64(r, k, n) < n) ++r;

    return pow_check_uint64(r, k, n) == n;
}

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
