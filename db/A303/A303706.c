/*
 * A303706  a(n) is the number of lattice points in a Cartesian grid between
 *          an equilateral triangle and an inscribed circle of radius n; one
 *          of the side of triangle is perpendicular to the X-axis; the
 *          circle's center is at the origin.
 * https://oeis.org/A303706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2·Σ_{m=1}^{3n-1} ⌊m/√3⌋ – 4·Σ_{x=1}^{n} ⌊√(n² – x²)⌋ – n
 *            – 1 (The sums are integer floors; the expression gives the
 *            number of lattice points that lie strictly inside the
 *            equilateral triangle of inradius n but outside the inscribed
 *            circle of radius n.)
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    /* integer square‑root (floor) for non‑negative n (n ≤ ~10¹⁰ fits in 64‑bit) */
    if (n < 2) return n;
    idx_t low = 1, high = n, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) { low = mid + 1; ans = mid; }
        else       { high = mid - 1; }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    seq_t sum_T = 0;                /* Σ floor(m/√3) for m = 1 … 3n‑1 */
    for (idx_t m = 1; m <= 3*n - 1; ++m) {
        idx_t m2 = m * m;           /* m² ≤ 9·10⁸ for n ≤ 10⁴  */
        idx_t v  = m2 / 3;          /* floor division */
        idx_t k  = isqrt_u64(v);    /* floor(√(m²/3)) = floor(m/√3) */
        sum_T += (seq_t)k;
    }

    seq_t sum_C = 0;                /* Σ floor(√(n² – x²)) for x = 1 … n */
    idx_t  n2   = n * n;            /* n² ≤ 10⁸  */
    for (idx_t x = 1; x <= n; ++x) {
        idx_t rem = n2 - x * x;
        idx_t k   = isqrt_u64(rem); /* floor(√(n² – x²)) */
        sum_C += (seq_t)k;
    }

    seq_t result = (seq_t)2 * sum_T
                 - (seq_t)4 * sum_C
                 - (seq_t)n
                 - 1;
    return result;
}
