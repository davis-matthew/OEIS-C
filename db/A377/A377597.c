/*
 * A377597  Table read by antidiagonals: T(n,k) = (n*k)!/(n^k*k!), n >=1, k >=
 *          0.
 * https://oeis.org/A377597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) where (m,k) are obtained by reading the table
 *            T(n,k) = (n*k)!/(n^k*k!) along its antidiagonals. The index n is
 *            mapped to coordinates (m,k) as follows: Find s = ceil(
 *            (sqrt(8*n+1)-1)/2 ), the diagonal containing n. Let prev =
 *            (s-1)*s/2. Then the offset within this diagonal is t = n -...
 * Range:     u8 n <= 16, u16 n <= 23, u32 n <= 31, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute T(n,k) = (n*k)!/(n^k*k!) via multiplicative method */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t start = n * (i - 1) + 1;
        idx_t end   = n * i;
        for (idx_t j = start; j <= end; ++j) {
            res *= (seq_t)j;
        }
        res /= (seq_t)n;   /* divide by n each step  */
        res /= (seq_t)i;   /* divide by i each step to account for k!  */
    }
    return res;
}

/* Find diagonal s such that s*(s+1)/2 >= idx  (1‑based) */
static idx_t diagonal_of(idx_t idx)
{
    idx_t low = 1;
    idx_t high = 1;
    /* increase high until it is large enough */
    while (1) {
        __uint128_t val = (__uint128_t)high * (high + 1) / 2;
        if (val >= idx) break;
        high <<= 1;
    }
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, harmless */
    /* Map sequence index to table coordinates (m,k) */
    idx_t s = diagonal_of(n);
    idx_t prev = (s - 1) * s / 2;
    idx_t t = n - prev - 1;         /* 0‑based offset within diagonal */
    idx_t m = t + 1;                /* row index (n in T)   */
    idx_t k = s - m;                /* column index (k in T) */
    return T_value(m, k);
}
