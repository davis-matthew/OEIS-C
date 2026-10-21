/*
 * A244136  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n^n as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            r*(r+1)/2 * T(r,0) = 1 if r==0 else 0 * T(r,k) = k^(k-1) *
 *            (r-k)^(r-k) for k>0 * * The function uses integer arithmetic
 *            only. It defines helper * routines for integer square root and
 *            fast exponentiation that work * for any seq_t (buil...
 * Range:     u8 n <= 15, u16 n <= 34, u32 n <= 66, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, used only in helper prototypes */

/* integer square root: floor(sqrt(x)) for 0 <= x < 2^64 */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    /* binary search with 128‑bit intermediate to avoid overflow */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;   /* avoid underflow on hi = mid-1 */
            hi = mid - 1;
        }
    }
    return hi;
}

/* fast exponentiation for any seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* OEIS A244136 */
seq_t a(idx_t n)
{
    /* compute row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t m   = 8*n + 1;                      /* 8*n+1 fits in idx_t for n<=10000 */
    idx_t sq  = isqrt_u64(m);                 /* floor(sqrt(8*n+1)) */
    idx_t r   = (sq - 1) / 2;
    idx_t start = r * (r + 1) / 2;            /* first index of row r */
    idx_t k     = n - start;                  /* column within the row */

    /* Handle T(r,0) */
    if (k == 0) {
        /* row 0 has T(0,0)=1, all other rows start with 0 */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* Compute k^(k-1) * (r-k)^(r-k) */
    seq_t term1 = pow_seq((seq_t)k,     k - 1);
    seq_t term2 = pow_seq((seq_t)(r - k), r - k);
    return term1 * term2;
}
