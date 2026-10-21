/*
 * A273496  Triangle read by rows: coefficients in the expansion cos(x)^n =
 *          (1/2)^n * Sum_{k=0..n} T(n,k) * cos(k*x).
 * https://oeis.org/A273496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r(r+1)/2 ≤ n < (r+1)(r+2)/2, T(r,k) is the
 *            coefficient in the expansion cos(x)^r = (1/2)^r Σ_{k=0..r}
 *            T(r,k) cos(kx)
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies                                                     */
#include <stdint.h>                     /* UINT64_MAX, uint64_t etc. */
#include "../utils/binomial.h"          /* binomial(idx_t n, idx_t k)   */

/* ------------------------------------------------------------------ */
/* Return the coefficient T(n,k) as defined in the OEIS entry         */
static seq_t T_coeff(idx_t n, idx_t k)
{
    /* (n-k) odd → zero */
    if (((n - k) & 1) != 0)
        return (seq_t)0;

    /* k == 0 case */
    if (k == 0) {
        if ((n & 1) == 0)               /* n even */
            return binomial(n, n / 2);
        else
            return (seq_t)0;
    }

    /* n-k even and k > 0 */
    idx_t m = (n - k) / 2;
    seq_t v = binomial(n, m);
    return v + v;                       /* multiply by 2 */
}

/* ------------------------------------------------------------------ */
/* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2                */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Exponential search for an upper bound */
    while (((unsigned __int128)high * (high + 1) / 2) <= n) {
        if (high > (UINT64_MAX >> 1))
            break;                      /* avoid overflow of shift left */
        high <<= 1;
    }

    /* Binary search between low and high */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 cumulative = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = (idx_t)(n - (idx_t)cumulative);
    return T_coeff(r, c);
}
