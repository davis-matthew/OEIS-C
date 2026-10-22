/*
 * A320239  Expansion of theta_3(q) * theta_3(q^3) * theta_3(q^5), where
 *          theta_3() is the Jacobi theta function.
 * https://oeis.org/A320239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (a1,a2,a3) such that a1^2 +
 *            3·a2^2 + 5·a3^2 = n (n ≥ 0). Each non‑zero coordinate has a ±
 *            sign, i.e. a solution with a1≠0 contributes 2, a2≠0 contributes
 *            2, a3≠0 contributes 2 to the count
 * Range:     u8 n <= 992, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for unsigned integer types using binary search. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n >> 1U;
    if (hi < 1) hi = 1;          /* ensure at least 1 for n=0,1 */
    if (n < 2) return n;         /* 0 or 1 */

    /* hi may be too big for n=2^63.. so cap it at 0xFFFFFFFF */
    if (hi > 0xFFFFFFFFULL) hi = 0xFFFFFFFFULL;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        else if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi; /* hi is floor sqrt(n) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* maximum a3 with 5*a3^2 ≤ n */
    idx_t max_a3 = isqrt_u64(n / 5U);
    for (idx_t a3 = 0; a3 <= max_a3; ++a3) {
        idx_t a3_sq = a3 * a3;                /* fits in idx_t */
        idx_t rem3  = n - 5U * a3_sq;         /* ≥ 0 */

        /* maximum a2 with 3*a2^2 ≤ rem3 */
        idx_t max_a2 = isqrt_u64(rem3 / 3U);
        for (idx_t a2 = 0; a2 <= max_a2; ++a2) {
            idx_t a2_sq = a2 * a2;
            idx_t rem2  = rem3 - 3U * a2_sq; /* ≥ 0 */

            /* a1^2 must equal rem2 */
            idx_t a1_sq = rem2;
            idx_t a1 = isqrt_u64(a1_sq);
            if (a1 * a1 == a1_sq) { /* perfect square */
                seq_t mult = 1;
                mult *= (a1 == 0 ? 1U : 2U);
                mult *= (a2 == 0 ? 1U : 2U);
                mult *= (a3 == 0 ? 1U : 2U);
                count += mult;
            }
        }
    }
    return count;
}
