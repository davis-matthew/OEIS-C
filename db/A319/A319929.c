/*
 * A319929  Minimal arithmetic table similar to multiplication with different
 *          rules for odd and even products, read by antidiagonals.
 * https://oeis.org/A319929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9211, u16 n <= 9211, u32 n <= 9211, u64 n <= 9211,
 *            u128 n <= 9211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A319929: read entries of table T(n,k) by antidiagonals.
 * Table definition:
 *   T(n,k) = n+k-1   if n odd  and k odd
 *   T(n,k) = n       if n even and k odd
 *   T(n,k) = k       if n odd  and k even
 *   T(n,k) = 0       if n even and k even
 * a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), …,
 * i.e. the antidiagonal traversal of the infinite table.
 * The function finds the diagonal d with d(d+1)/2 ≥ n
 * and returns the appropriate table entry.  Complexity O(log n). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n<1 is irrelevant */

    /* Find smallest d such that d*(d+1)/2 ≥ n */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;

    lo = hi >> 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;  /* diagonal index (i + j - 1) */

    /* Compute position inside the diagonal */
    idx_t prev_sum = (idx_t)(((unsigned __int128)(d - 1) * d) / 2);
    idx_t offset = n - prev_sum - 1; /* zero‑based offset along this diagonal */
    idx_t i = offset + 1;
    idx_t j = d - i + 1;

    /* Evaluate T(i,j) using seq_t arithmetic */
    if ((i & 1ULL) == 0) {            /* i even */
        if ((j & 1ULL) == 0)
            return (seq_t)0;
        else
            return (seq_t)i;
    } else {                          /* i odd */
        if ((j & 1ULL) == 0)
            return (seq_t)j;
        else
            return (seq_t)i + (seq_t)j - (seq_t)1;
    }
}
