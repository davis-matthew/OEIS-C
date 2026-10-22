/*
 * A330381  Triangle read by rows: T(n,k) is the number of ternary strings of
 *          length n with k indispensable digits, with 0 <= k <= n.
 * https://oeis.org/A330381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r, k); r = floor((sqrt(8*n+1)-1)/2),
 *             k = n - r*(r+1)/2.
 *         T(r, k) = A(r, 2*k-1) + A(r, 2*k).
 *         A(r, m) = Σ_{j=0}^{⌊m/3⌋} (-1)^j * C(r, j) * C(m-3*j + r - 1, r - 1).
 */
#include <math.h>
#include "../utils/binom.h"

/* Compute A(r, m): number of length‑r ternary strings with digit sum m */
static seq_t compute_A(idx_t r, idx_t m)
{
    if (m > 2*r) return 0;
    if (r == 0) return (m == 0) ? 1 : 0;

    seq_t sum = 0;
    idx_t maxj = m / 3;           /* floor(m/3) */
    for (idx_t j = 0; j <= maxj; ++j) {
        /* terms of Inclusion–Exclusion */
        seq_t term = binom(r, j) * binom(m - 3*j + r - 1, r - 1);
        if (j & 1) sum -= term;
        else      sum += term;
    }
    return sum;
}

/* Compute T(r, k) */
static seq_t compute_T(idx_t r, idx_t k)
{
    seq_t res = compute_A(r, 2*k);
    if (k > 0) res += compute_A(r, 2*k - 1);
    return res;
}

/* Find the row r that contains the index n (0‑based) */
static idx_t row_for_index(idx_t n)
{
    /* r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    long double approx = sqrtl(2.0L * (long double)n);
    idx_t r = (idx_t)approx;

    /* correct possible off‑by‑one errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = row_for_index(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* column within the row */
    return compute_T(r, k);
}
