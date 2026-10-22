/*
 * A394824  Triangle read by rows: T(n, k) = n^(n - k) * hypergeom([k, k-n],
 *          [], -1/n) for n > 0, T(0, 0) = 1.
 * https://oeis.org/A394824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A394824 – Triangle read by rows
 *
 *  T(n, k) = n^(n - k) * Hypergeom([k, k-n], [], -1/n)  (n > 0, 0 ≤ k ≤ n)
 *  Recurrence:  T(n, k) = (2k + 1) * T(n, k+1) + (k+1)(n-k-1) * T(n, k+2)
 *  The sequence a(n) is obtained by listing the triangle row‑by‑row:
 *      a(0)=T(0,0),  a(1)=T(1,0),  a(2)=T(1,1),  a(3)=T(2,0), …
 *
 *  Implementation uses the recurrence above.  For each query a(n) we
 *  first locate the row m satisfying m(m+1)/2 ≤ n < (m+1)(m+2)/2,
 *  then compute the required entry T(m, j) with j = n - m(m+1)/2.
 *
 *  All arithmetic is performed on the custom integer types seq_t and
 *  idx_t supplied by the build system.  No floating‑point or large
 *  temporary values are used; an integer square‑root routine is
 *  provided within this file.
 *
 *  Complexity:  O(m) per call, where m ≈ √(2n).
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t result = 0;
    idx_t bit    = 1ULL << 62;   /* second‑top bit of 64‑bit integer */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= result + bit) {
            n -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

static seq_t compute_T(idx_t n, idx_t k)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;          /* T(0,0) = 1 */
    if (n == 1) return (seq_t)1;          /* T(1,0) = T(1,1) = 1 */

    if (k == n)        return (seq_t)1;          /* diagonal */
    if (k == n - 1)    return (seq_t)(2 * n - 1);/* second diagonal */

    /* Backward recurrence */
    seq_t t_next2 = (seq_t)1;
    seq_t t_next1 = (seq_t)(2 * n - 1);  /* T(n, n-1) */

    for (idx_t km = n - 2; km < n; km--)   /* loops until km == 0 */
    {
        seq_t t_curr = (seq_t)(2 * km + 1) * t_next1
                     + (seq_t)(km + 1) * (seq_t)(n - km - 1) * t_next2;

        if (km == k) return t_curr;

        t_next2 = t_next1;
        t_next1 = t_curr;
    }

    /* Should never reach here */
    return (seq_t)1;
}

seq_t a(idx_t n)
{
    /* Find the row m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2.  */
    idx_t temp = n * 8 + 1;          /* 8n + 1 */
    idx_t sqrtv = isqrt_u64(temp);
    idx_t m = (sqrtv - 1) / 2;        /* floor((sqrt(8n+1)-1)/2) */

    idx_t tri = m * (m + 1) / 2;      /* triangular number m(m+1)/2 */
    idx_t j   = n - tri;              /* column within the row */

    return compute_T(m, j);
}
