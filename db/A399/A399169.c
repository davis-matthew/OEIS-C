/*
 * A399169  Number of solutions to x^2 + 4*y^2 = n in positive integers x and
 *          y.
 * https://oeis.org/A399169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{(x,y)∈ℕ⁺×ℕ⁺ | x² + 4 y² = n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic and a simple
 * binary‑search based integer square‑root. */

static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n, res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {          /* avoid division by zero */
            res = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* No positive solutions for n < 5 (minimum 4 · 1² + 1² = 5). */
    if (n < 5) return 0;

    seq_t count = 0;
    /* Maximum y satisfies 4 y² ≤ n → y ≤ ⌊√(n/4)⌋. */
    idx_t max_y = isqrt_u64(n / 4);

    for (idx_t y = 1; y <= max_y; ++y) {
        /* Compute 4 y² using 128‑bit to avoid overflow. */
        __int128 t = (__int128)4 * y * y;
        idx_t rem = n - (idx_t)t;   /* congruent to n – 4 y² */

        if (rem == 0) continue;    /* would give x = 0, not positive */

        idx_t r = isqrt_u64(rem);
        if (r * r == rem)          /* rem is a perfect square */
            ++count;
    }

    return count;
}
