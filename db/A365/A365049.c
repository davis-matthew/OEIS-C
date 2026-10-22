/*
 * A365049  a(n) is the number of distinct parallelograms with integer sides
 *          and area n, and where at least one height is an integer.
 * https://oeis.org/A365049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts distinct integer‑sided parallelograms with area n
 *            and at least one integer height. The implementation follows the
 *            OEIS description using divisors and the number of Pythagorean
 *            triples with a given leg
 * Range:     u8 n <= 15, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values
   (no floating point, no math.h). */
static inline idx_t int_sqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    idx_t best = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = (idx_t)mid * (idx_t)mid;
        if (sq == x) return mid;
        if (sq < x) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* Count Pythagorean triples having the given integer as a leg.
   For leg m we enumerate all divisors a of m² with a ≤ √(m²).
   For each a, set b = m² / a.  If a,b have the same parity
   and a ≠ b, a unique triple exists. */
static seq_t count_pyth_leg(idx_t m)
{
    if (m <= 1) return 0;
    seq_t m2 = (seq_t)m * (seq_t)m;
    idx_t sqrt_m2 = int_sqrt_u64((idx_t)m2);
    seq_t cnt = 0;
    for (idx_t a = 1; a <= sqrt_m2; ++a) {
        if (m2 % a != 0) continue;
        idx_t b = (idx_t)(m2 / a);
        if ((a & 1) == (b & 1) && a != b) {
            ++cnt;   /* one Pythagorean triple */
        }
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    idx_t sqrt_n = int_sqrt_u64(n);
    bool is_square = (sqrt_n * sqrt_n == n);
    seq_t res = 0;

    if (is_square) {
        /* Square case: count the square rectangle and its one set of extra
           parallelograms, then handle all proper factor pairs. */
        res += 1 + count_pyth_leg(sqrt_n);
        for (idx_t d = 1; d < sqrt_n; ++d) {
            if (n % d != 0) continue;
            idx_t e = n / d;          /* e == sqrt_n for d == sqrt_n, skip that */
            res += 1 + count_pyth_leg(d) + count_pyth_leg(e);
        }
    } else {
        /* Non‑square case: iterate all divisor pairs (d,e) with d ≤ e. */
        idx_t limit = sqrt_n;
        for (idx_t d = 1; d <= limit; ++d) {
            if (n % d != 0) continue;
            idx_t e = n / d;
            res += 1 + count_pyth_leg(d) + count_pyth_leg(e);
        }
    }

    return res;
}
