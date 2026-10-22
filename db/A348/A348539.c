/*
 * A348539  Triangle T(n, m) = binomial(n+2, m)*binomial(n+2, n-m), read by
 *          rows.
 * https://oeis.org/A348539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r+2, m)*binomial(r+2, r-m) where r =
 *            ⌊(√(8*n+1)-1)/2⌋, m = n - r(r+1)/2 The triangle T(n,m) =
 *            C(n+2,m)*C(n+2,n-m) read by rows
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: integer square root of an idx_t value (floor). */
static idx_t isqrt_u64(idx_t x)
{
    if (x < 2)
        return x;
    /* Highest set bit position (0-based) */
    idx_t len = 64 - __builtin_clzll(x);
    /* Upper bound for sqrt(x) */
    idx_t lo = 0;
    idx_t hi = ((idx_t)1) << ((len + 1) / 2);
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1; /* lo is first > sqrt */
}

/* Helper: binomial coefficient C(n,k) computed multiplicatively */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;          /* exact division */
    }
    return res;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Determine row r */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    /* Position within the row */
    idx_t m = n - r * (r + 1) / 2;
    /* Compute the two binomial factors */
    seq_t a1 = binom_calc(r + 2, m);
    seq_t a2 = binom_calc(r + 2, r - m);
    return a1 * a2;
}
