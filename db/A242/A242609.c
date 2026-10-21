/*
 * A242609  Expansion of phi(-q) * phi(q^8) in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A242609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k,l∈ℤ} (-1)^k , with k² + 8·l² = n * (coefficients of
 *            φ(−q)·φ(q⁸) )
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* integer square root: floor(sqrt(x)), works for all 64‑bit unsigned x */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*mid may overflow, so compare via division */
        if (mid > 0 && mid > x / mid) {        /* mid*mid > x */
            high = mid - 1;
        } else {
            if (mid * mid == x) return mid;    /* exact match */
            low = mid + 1;
        }
    }
    return high;                                /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* maximum absolute value of l such that 8·l² ≤ n */
    idx_t max_l = isqrt_u64(n / 8);

    for (idx_t l = 0; l <= max_l; ++l) {
        /* remaining part that must be a perfect square */
        idx_t rest = n - 8 * l * l;
        if (rest == 0) {          /* k = 0 is the only possibility */
            result += (l == 0) ? (seq_t)1 : (seq_t)2;
            continue;
        }
        idx_t k = isqrt_u64(rest);
        if (k * k != rest) continue; /* not a square */

        /* sign = (-1)^k */
        seq_t sign = (k & 1) ? (seq_t)-1 : (seq_t)1;

        if (l == 0) {
            result += 2 * sign;         /* k = ±k, l = 0 */
        } else {
            result += 4 * sign;         /* k = ±k, l = ±l */
        }
    }
    return result;
}
