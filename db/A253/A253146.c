/*
 * A253146  A fractal tree, read by rows: for n > 2, T(n,1) = T(n-1,1)+2,
 *          T(n,n) = T(n-1,1)+3, and for k=2..n-1, T(n,k) = T(n-2,k-1).
 * https://oeis.org/A253146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T is the “fractal tree” defined by T(1,1)=1;
 *            for n>2: T(n,1)=T(n-1,1)+2; T(n,n)=T(n-1,1)+3;
 *            T(n,k)=T(n-2,k-1). Closed‑form: let r = ceil((sqrt(8n+1)-1)/2),
 *            k = n - r(r-1)/2. If r ≥ 2k-1 → a(n)=2*(r-2(k-1)-1), otherwise
 *            a(n)=2*(2k-r)-1
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety, unused for OEIS offset 1 */

    /* Find row r: minimal r with r(r+1)/2 ≥ n */
    long double ld = sqrt((long double)(8.0L * n + 1.0L));
    idx_t r = (idx_t)((ld - 1.0L) / 2.0L);
    /* Adjust for possible rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) --r;

    /* Column index k (1‑based) in this row */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - prev;

    /* Compute T(r,k) using the closed form derived from the recurrence */
    if (r >= 2 * k - 1) {
        idx_t nprime = r - 2 * (k - 1);
        if (nprime == 1) return (seq_t)1;
        return (seq_t)(2 * (nprime - 1));
    } else {
        idx_t nprime = 2 * k - r;
        return (seq_t)(2 * nprime - 1);
    }
}
