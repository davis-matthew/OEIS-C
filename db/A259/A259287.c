/*
 * A259287  Expansion of psi(x^2) * f(x^3, x^5) in powers of x where psi(),
 *          f(, ) are Ramanujan theta functions.
 * https://oeis.org/A259287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (u,v) such that 16*n + 5 =
 *            (8*u + 1)^2 + (8*v + 2)^2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for uint64_t, used for all operations. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, mid, ans = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if (mid == 0) {          /* avoid division by zero */
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {    /* mid*mid <= n  without overflow */
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* target = 16*n + 5, fits in idx_t even for large n */
    idx_t target = 16 * n + 5;

    /* Compute maximum |8*u + 1| <= sqrt(target) */
    idx_t sqrt_target = isqrt_u64(target);
    idx_t u_limit = sqrt_target / 8 + 1;     /* safe upper bound */

    seq_t count = 0;

    /* Iterate over all integer u in [-u_limit, u_limit] */
    for (int64_t u = -(int64_t)u_limit; u <= (int64_t)u_limit; ++u) {
        int64_t a_val = 8 * u + 1;               /* 8*u + 1 */
        int64_t a_sq  = a_val * a_val;           /* square, fits in 64‑bit */

        if ((idx_t)a_sq > target) continue;      /* too large */

        idx_t rem = target - (idx_t)a_sq;        /* remaining part for b^2 */

        idx_t sqrt_b = isqrt_u64(rem);
        if (sqrt_b * sqrt_b != rem) continue;    /* not a perfect square */

        /* Check positive root b = +sqrt_b */
        int64_t b_pos = (int64_t)sqrt_b;
        if ((b_pos - 2) % 8 == 0) count++;

        /* Check negative root b = -sqrt_b, only if sqrt_b != 0 */
        if (sqrt_b != 0) {
            int64_t b_neg = -(int64_t)sqrt_b;
            if ((b_neg - 2) % 8 == 0) count++;
        }
    }

    return (seq_t)count;
}
