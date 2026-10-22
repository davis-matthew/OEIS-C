/*
 * A368521  Triangular array T, read by rows: T(n,k) = number of sums |x-y| +
 *          |y-z| - |x-z| = k, where x,y,z are in {1,2,...,n}.
 * https://oeis.org/A368521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is the triangular array read by rows. Row n has
 *            entries T(n,2d) for d = 0 .. n-1: T(n,0) = n*(n² + 3n - 1)/3
 *            T(n,2d) = 2*(n - d)² for d > 0 a(n) is the (n‑th) entry in the
 *            row‑major order of this array
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* convert to zero‑based index */
    uint64_t idx = (uint64_t)n - 1;

    /* find row r such that r*(r+1)/2 >= idx+1 */
    uint64_t r = 1;
    double approx = (sqrt(8.0 * idx + 1.0) - 1.0) / 2.0;
    r = (uint64_t)approx;
    /* adjust to ensure correct r */
    while ((unsigned __int128)r * (r + 1) / 2 <= idx) r++;
    while (r > 1 && ((unsigned __int128)(r - 1) * r / 2) > idx) r--;

    /* position inside the row */
    uint64_t prev = (r - 1) * r / 2;          /* number of entries before this row */
    uint64_t c = idx - prev + 1;              /* column index, 1‑based */
    uint64_t d = c - 1;                       /* d = (k/2) */

    /* compute the value using the formulas above */
    seq_t result;
    if (d == 0) {
        seq_t rn  = (seq_t)r;
        seq_t n2  = rn * rn;
        seq_t num = rn * (n2 + 3 * rn - 1);
        result = num / 3;
    } else {
        seq_t diff = (seq_t)(r - d);
        result = 2 * diff * diff;
    }

    return result;
}
