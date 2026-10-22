/*
 * A384447  Array read by ascending antidiagonals: A(n, k) = gcd(n, k) if n >
 *          0 otherwise 0.
 * https://oeis.org/A384447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the matrix A(n,k)=gcd(n,k) by ascending
 *            antidiagonals. For n>0, A(n,k)=gcd(n,k); for n=0 all entries are
 *            0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for arbitrary n (0‑based). */
#include "../utils/gcd.h"

/* Helper: integer square root, floor(sqrt(n)) for idx_t. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* Handle n == 0 directly; all entries with row 0 are 0. */
    if (n == 0) return 0;

    /* Determine the antidiagonal s such that the n-th element lies in it. */
    /* s satisfies: s(s+1)/2 <= n < (s+1)(s+2)/2. */
    idx_t s = (isqrt_u64(1 + 8 * n) - 1) >> 1;

    /* Offset within the diagonal. */
    idx_t offset = n - (s * (s + 1) / 2);

    /* Compute row (n_val) and column (k_val) indices for the pair. */
    idx_t n_val = s - offset;   /* row index */
    idx_t k_val = offset;       /* column index */

    /* For row 0, the value is 0 (by definition). */
    if (n_val == 0) return 0;

    /* Compute gcd of the pair. */
    return gcd((seq_t)n_val, (seq_t)k_val);
}
