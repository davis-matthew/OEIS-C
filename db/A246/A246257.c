/*
 * A246257  Triangular array read by rows: T(n, k) = S(n, [n/2]-k) and S(n,k)
 *          = C(n, 2*k)*(2*k-1)!!*((2*k-1)!! + 1)/2, n>=0, 0<=k<=[n/2].
 * https://oeis.org/A246257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) is the n-th entry (row-major) of the triangle T(n, k) =
 *            S(n, floor(n/2)-k), S(n,k) = C(n,2k)*(2k-1)!!*((2k-1)!!+1)/2.
 *            The sequence itself is index by rows: a(0)=T(0,0), a(1)=T(1,0),
 *            a(2)=T(2,0), a(3)=T(2,1), a(4)=T(3,0), … This implementation
 *            finds the corresponding row r and column k from i...
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, double_factorial_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/double_factorial_odd.h"

/* cumulative number of triangle entries up to and including row r */
static inline idx_t cumulative_entries(idx_t r) {
    if (r & 1) { /* r is odd: r = 2m+1 */
        idx_t m = (r - 1) / 2;
        return (m + 1) * (m + 2);
    } else {     /* r is even: r = 2m */
        idx_t m = r / 2;
        return (m + 1) * (m + 1);
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Find row r such that cumulative_entries(r-1) <= n < cumulative_entries(r) */
    idx_t r = 0;
    while (cumulative_entries(r) <= n) {
        ++r;
    }
    idx_t cum_prev = (r == 0) ? 0 : cumulative_entries(r - 1);
    idx_t k = n - cum_prev;               /* column index within the row */

    idx_t half = r >> 1;                  /* floor(r/2) */
    idx_t j    = half - k;                /* index for S */

    /* Compute S(r, j) = C(r, 2j) * d * (d+1) / 2, where d = (2j-1)!! */
    seq_t bin = binomial(r, 2 * j);
    seq_t d   = double_factorial_odd(j);
    seq_t val = bin * d;
    val = val * (d + 1) / 2;
    return val;
}
