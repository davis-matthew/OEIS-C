/*
 * A379611  Table read by rows: T(n, k) = (n + 1)^(n - 1) - (k - 1)*(n + 1)^(n
 *          - 2), by convention T(1, 0) = 1.
 * https://oeis.org/A379611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1) - 1)/2), // row index
 *            k = n - r*(r+1)/2, // column index in the row T(r,k) =
 *            (r+1)^(r-2) * (r - k + 2) for r ≥ 2, T(1,0) = T(1,1) = 1, T(0,0)
 *            = 2. The function below computes a(n) for any n ≥ 0
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1ULL)
            result *= base;
        exp >>= 1ULL;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Binary search for r where r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = (1ULL << 32) + 2;          /* safe upper bound    */

    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        idx_t total;
        if ((mid & 1ULL) == 0)
            total = (mid >> 1) * (mid + 1);
        else
            total = mid * ((mid + 1) >> 1);

        if (total <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r = low;                          /* row index   */
    idx_t prev_total;                       /* number of elements before row r */
    if ((r & 1ULL) == 0)
        prev_total = (r >> 1) * (r + 1);
    else
        prev_total = r * ((r + 1) >> 1);

    idx_t k = n - prev_total;               /* column index */

    /* Special cases */
    if (r == 0)
        return (seq_t)2;

    if (r == 1)
        return (seq_t)1;

    /* General formula for r ≥ 2 */
    seq_t base  = (seq_t)(r + 1);
    idx_t  exp   = r - 2;
    seq_t pow_base = pow_seq(base, exp);
    seq_t factor  = (seq_t)(r - k + 2);

    return pow_base * factor;
}
