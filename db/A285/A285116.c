/*
 * A285116  Triangle read by rows: T(0,n) = T(n,n) = 1; and for n > 0, 0 < k <
 *          n, T(n,k) = C(n-1,k-1) OR C(n-1,k), where C(n,k) is binomial
 *          coefficient (A007318) and OR is bitwise-OR (A003986).
 * https://oeis.org/A285116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,n) = T(n,n) = 1; for 0<k<n, T(n,k) = C(n-1,k-1) OR C(n-1,k).
 *            a(n) is the row‑wise reading of the triangle
 * Range:     u8 n <= 81, u16 n <= 217, u32 n <= 682, u64 n <= 2445,
 *            u128 n <= 8973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t _row_from_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Grow high until it exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    /* Binary search on [low, high) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Binomial coefficient C(n,k) for idx_t arguments, result in seq_t.
   Handles both fixed‑size integer seq_t (via __int128 intermediate)
   and arbitrary precision seq_t (via native operations). */
static inline seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
#if defined(__cplusplus)
        /* In C++ use constexpr to choose implementation based on sizeof(seq_t) */
        if constexpr (sizeof(seq_t) <= 8) {
            res = (seq_t)((unsigned __int128)res * (seq_t)(n - k + i) / (seq_t)i);
        } else {
            res = res * (seq_t)(n - k + i) / (seq_t)i;
        }
#else
        /* In C assume seq_t is a fixed width integer */
        res = (seq_t)((unsigned __int128)res * (seq_t)(n - k + i) / (seq_t)i);
#endif
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = _row_from_index(n);         /* row number */
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t p = n - tri;                    /* position in row, 0‑based */

    /* Boundary cases */
    if (p == 0 || p == r) return (seq_t)1;

    seq_t left  = _binom(r - 1, p - 1);   /* C(r-1, p-1) */
    seq_t right = _binom(r - 1, p);       /* C(r-1, p)   */
    return left | right;
}
