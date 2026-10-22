/*
 * A370233  Triangle read by rows. T(n, k) = (n - k + 1) * binomial(n + k + 1,
 *          2*k)^2 / (n + k + 1).
 * https://oeis.org/A370233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r - k + 1) * binomial(r + k + 1, 2*k)^2 / (r + k + 1)
 *            where r = floor((sqrt(8*n+1) - 1)/2) and k = n - r*(r+1)/2 – the
 *            triangle read by rows of A370233
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned value.  */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Compute binomial coefficient C(n,k) using multiplicative formula.  */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Determine row r using triangular numbers.  */
    idx_t r = (floor_sqrt_u64(8 * n + 1) - 1) >> 1;

    /* Correct r in case of off‑by‑one from the floor sqrt.  */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t k = n - r * (r + 1) / 2;            /* 0 ≤ k ≤ r */

    /* T(r,k) = (r-k+1) * binomial(r+k+1,2k)^2 / (r+k+1).  */
    seq_t choose = binom(r + k + 1, 2 * k);
    seq_t val = choose * choose;              /* choose^2 */
    val *= (r - k + 1);
    val /= (r + k + 1);

    return val;
}
