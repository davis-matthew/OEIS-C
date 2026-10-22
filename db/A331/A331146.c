/*
 * A331146  Triangle read by rows: T(n,k) (n>=k>=1) = f(n,n-k+1) where f(n,k)
 *          = ceiling((n/k)*ceiling(n/k)).
 * https://oeis.org/A331146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle read by rows T(n,k) = f(n,n-k+1) with f(n,k) =
 *            ceil((n/k) * ceil(n/k)) a(n) enumerates the triangle by rows:
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), …
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/triangular.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined; a(1) is the first term */

    /* find the row r such that r(r+1)/2 >= n */
    idx_t low = 1, high = 2;
    while (triangular(high) < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* position within the row */
    idx_t prev = triangular(r - 1);
    idx_t k = n - prev;                /* 1 ≤ k ≤ r */
    idx_t j = r - k + 1;               /* argument of f in the problem */

    /* compute f(r, j) = ceil((r/j) * ceil(r/j)) */
    seq_t rr = (seq_t)r;
    seq_t jj = (seq_t)j;
    seq_t q = (rr + jj - 1) / jj;      /* ceil(r/j) */
    seq_t prod = rr * q;               /* r * ceil(r/j) */
    seq_t res = (prod + jj - 1) / jj;  /* ceil(prod / j) */

    return res;
}
