/*
 * A359250  Irregular triangle read by rows where T(n,k) is the coefficient of
 *          y^k in polynomial P(n) defined by P(2n) = P(n) and P(2n+1) =
 *          y*P(n) + P(n+1) starting P(0) = 0, P(1) = 1.
 * https://oeis.org/A359250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, easy
 * Formula:   a(n) = coefficient of y^k in the polynomial P(r) defined by *
 *            P(2n) = P(n), * P(2n+1) = y*P(n) + P(n+1), P(0)=0, P(1)=1. *
 *            Here r is the unique integer with * ∑_{i=1}^{r-1} popcount(i) ≤
 *            n < ∑_{i=1}^{r} popcount(i), * and k = n - ∑_{i=1}^{r-1}
 *            popcount(i)
 * Range:     u8 n <= 87, u16 n <= 11263, u32 n <= 11263, u64 n <= 11263,
 *            u128 n <= 11263, big any n
 * Validated: standard, pass, 104 terms through n = 9766 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of popcounts of all numbers 0..n (inclusive). */
static inline seq_t popcount_sum_upto(idx_t n)
{
    seq_t sum = 0;
    for (int bit = 0; bit < 64; ++bit) {
        idx_t bit_mask = ((idx_t)1) << bit;
        if (bit_mask > n) break;
        idx_t block = bit_mask << 1;            /* 2^(bit+1) */
        idx_t full_cycles = (n + 1) / block;
        idx_t ones_in_full = full_cycles * bit_mask;
        sum += (seq_t)ones_in_full;
        idx_t rem = (n + 1) % block;
        if (rem > bit_mask) sum += (seq_t)(rem - bit_mask);
    }
    return sum;
}
/* Helper to compute popcount of a 64‑bit integer */
static inline uint64_t popcount_u64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    uint64_t c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* Recursively compute coefficient T(n,k). */
static seq_t T_rec(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (k == 0) ? (seq_t)1 : (seq_t)0;
    if ((n & 1) == 0) { /* even n */
        return T_rec(n >> 1, k);
    } else {            /* odd n = 2*m + 1 */
        idx_t m = n >> 1;            /* (n-1)/2 */
        seq_t res = 0;
        if (k > 0) res += T_rec(m, k - 1);
        res += T_rec(m + 1, k);
        return res;
    }
}

/* Find the row r such that popcount_sum_upto(r-1) ≤ idx < popcount_sum_upto(r). */
static idx_t find_row(idx_t idx)
{
    idx_t lo = 1, hi = 1;
    while (popcount_sum_upto(hi) <= idx) hi <<= 1; /* exponential search for upper bound */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (popcount_sum_upto(mid) > idx)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n is a sequence index (offset 0). */
    if (n == 0) {
        /* The very first term corresponds to row 1, position 0. */
        return (seq_t)1;
    }

    /* Find the triangle row r containing the n‑th term. */
    idx_t row = find_row(n);

    /* Compute position p within that row (0‑based). */
    seq_t before = popcount_sum_upto(row - 1);
    idx_t pos = (idx_t)(n - before); /* pos < popcount(row) */

    return T_rec(row, pos);
}
