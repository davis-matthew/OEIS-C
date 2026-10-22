/*
 * A343847  T(n, k) = (n - k)! * [x^(n-k)] exp(k*x/(1 - x))/(1 - x). Triangle
 *          read by rows, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A343847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element when the triangle T(n,k) with T(n,k) =
 *            (n-k)! * Σ_{j=0}^{n-k} binomial(n-k, j) * k^j / j! is read by
 *            rows (row 0: T(0,0), row 1: T(1,0),T(1,1), …). This
 *            implementation finds the row (r) and column (k) of the requested
 *            index n, then computes T(r,k) using the recurrence T(n,...
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        unsigned __int128 cum = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (cum > n) break;
        r++;
    }
    unsigned __int128 prev_cum = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)prev_cum;

    /* base cases */
    if (k == r)          /* n == k -> T(k,k) = 1 */
        return (seq_t)1;
    if (k == r - 1)      /* n == k+1 -> T(k+1,k) = k+1 */
        return (seq_t)(k + 1);

    /* recurrence for T(n,k) along the column k */
    seq_t t_prevprev = (seq_t)1;                     /* T(k,k) */
    seq_t t_prev     = (seq_t)(k + 1);               /* T(k+1,k) */

    for (idx_t cur = k + 2; cur <= r; ++cur) {
        idx_t term1 = 2 * cur - k - 1;               /* 2*cur - k - 1 */
        idx_t diff   = cur - k - 1;                  /* cur - k - 1 */
        idx_t term2 = diff * diff;                  /* (cur - k - 1)^2 */
        seq_t term_mul1 = (seq_t)term1 * t_prev;     /* (2*cur - k - 1) * T(cur-1,k) */
        seq_t term_mul2 = (seq_t)term2 * t_prevprev; /* (cur - k - 1)^2 * T(cur-2,k) */
        seq_t t_cur = term_mul1 - term_mul2;         /* recurrence */
        t_prevprev = t_prev;
        t_prev = t_cur;
    }
    return t_prev;
}
