/*
 * A367025  Triangle read by rows, T(n, k) = [x^k] p(n), where p(n) = (1 -
 *          hypergeom([-1/2, -n - 1, -n - 1], [1, 1], 4*x)) / (2*x).
 * https://oeis.org/A367025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle T(n,k) = [x^k]((1 -
 *            hypergeom([-1/2, -n-1, -n-1], [1,1], 4x))/(2x)) where T(n,k) =
 *            binomial(n+1, n-k)^2 * binomial(2k, k)/(k+1), read by rows (row
 *            n has k = 0..n)
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t triangular_row(idx_t n)
{
    idx_t lo = 0, hi = n + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

static seq_t catalan(idx_t k)
{
    if (k == 0) return 1;
    seq_t res = binom(2 * k, k);
    res = res / (k + 1);
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_row(n);
    idx_t row_start = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t k = n - row_start;

    seq_t bin = binom(r + 1, k + 1);          // C(r+1, k+1)
    seq_t bin_sq = bin * bin;                // square
    seq_t cat = catalan(k);                  // Catalan(k)
    return bin_sq * cat;
}
