/*
 * A277951  Triangle read by rows, in which row n gives coefficients in
 *          expansion of ((x^n - 1)/(x - 1))^6.
 * https://oeis.org/A277951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n‑th term of the triangle read by rows of
 *            ((x^row-1)/(x-1))^6, i.e. T(row, k) where k is the position
 *            within that row. The coefficient can be computed by the
 *            alternating sum: T(row, k) = Σ_{i=0}^{⌊k/row⌋} (-1)^i * C(6,i) *
 *            C(k+5-i·row,5). This function finds the row containing the
 *            n‑t...
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 7284, u64 n <= 7400,
 *            u128 n <= 7400, big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mul_small.h"

static inline idx_t cum_count(idx_t r) {
    /* cumulative number of terms up to row r: r*(3r-2) */
    return r * (3 * r - 2);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n<1, but safe */

    /* Find the row r such that cum_count(r-1) < n ≤ cum_count(r) */
    idx_t low = 1, high = 1;
    while (cum_count(high) < n) high <<= 1;   /* exponential search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_count(mid) >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* previous cumulative count up to row r-1 */
    idx_t prev = (r == 1) ? 0 : cum_count(r - 1);

    /* 0‑based index within the row */
    idx_t k = n - prev - 1;

    /* Pre‑calculated C(6,i) for i = 0..6 */
    static const unsigned small_binom[7] = {1, 6, 15, 20, 15, 6, 1};

    seq_t pos_sum = 0;  /* sum of terms with even i (positive) */
    seq_t neg_sum = 0;  /* sum of terms with odd i (negative) */

    idx_t max_i = k / r; /* floor(k / r) */
    for (idx_t i = 0; i <= max_i; ++i) {
        /* Compute binomial(k + 5 - i·r, 5) only if the top >= 5 */
        idx_t top = k + 5;
        idx_t sub = i * r;
        if (top < sub) continue;

        seq_t term = binomial(top - sub, 5);
        term = mul_small(term, small_binom[i]);

        if ((i & 1) == 0)
            pos_sum += term;
        else
            neg_sum += term;
    }

    return pos_sum - neg_sum;
}
