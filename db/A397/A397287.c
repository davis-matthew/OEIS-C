/*
 * A397287  Triangle read by rows: T(n, k) = k*(k + n)^(n - 1) for n > 0 and
 *          T(0, k) = 1.
 * https://oeis.org/A397287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = k * (k + r)^(r-1), r =
 *            floor((sqrt(8*n + 1) - 1)/2), k = n - r*(r+1)/2. Triangle read
 *            by rows, offset 0
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 49, u64 n <= 116,
 *            u128 n <= 317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for 64‑bit unsigned values. */
static inline idx_t int_sqrt_uint64(idx_t v)
{
    idx_t left  = 0;
    idx_t right = (idx_t)1 << 32;            /* > sqrt(UINT64_MAX) */
    while (left < right)
    {
        idx_t mid = left + ((right - left) >> 1);
        if (mid > 0 && mid > v / mid)   /* mid^2 > v */
            right = mid;
        else
            left = mid + 1;
    }
    return left - 1;
}

/* Power: base^exp, exponent is non‑negative. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Determine row r. */
    idx_t m = (n << 3) + 1;          /* 8*n + 1 */
    idx_t s = int_sqrt_uint64(m);    /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) >> 1;          /* (s-1)/2 */

    /* Compute index within the row: k = n - sum_{i=0}^{r-1} (i+1). */
    idx_t a = r, b = r + 1;
    if (a & 1) b >>= 1; else a >>= 1;   /* divide the even factor by 2 */
    idx_t prev = a * b;                 /* r*(r+1)/2 */
    idx_t k = n - prev;

    /* Evaluate T(r,k).  For r>0, k==0 gives 0 automatically. */
    if (k == 0) return (seq_t)0;        /* covers all r>0 cases with k=0 */

    seq_t base = (seq_t)(k + r);
    seq_t powv = pow_seq(base, r - 1);
    return (seq_t)k * powv;
}
