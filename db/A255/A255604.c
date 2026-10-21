/*
 * A255604  Table read by antidiagonals, T(n,k) is the integer part of the
 *          area of a regular k-gon with side length n.
 * https://oeis.org/A255604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor( k*n^2/(4*tan(π/k)) ) read by antidiagonals. Here n
 *            and k are derived from the index n of the OEIS sequence: a(n) =
 *            T(n',k) with (n',k) given by the antidiagonal enumeration
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static const double PI = acos(-1.0);

/* Return the n-th term of OEIS A255604 (1‑based index). */
seq_t a(idx_t N)
{
    if (N == 0) return 0;   /* Indexing starts at 1, but guard anyway. */

    /* Find the diagonal t such that t*(t+1)/2 >= N. */
    idx_t t = 1;
    while ((t * (t + 1)) / 2 < N) ++t;

    /* Number of terms before this diagonal. */
    idx_t prev = (t - 1) * t / 2;

    /* Zero‑based offset within the current diagonal. */
    idx_t offset = N - prev - 1;

    /* Side length decreases while k increases along the diagonal. */
    idx_t n = t - offset;         /* n >= 1 */
    idx_t k = offset + 3;         /* k >= 3 */

    /* Compute the floor of k*n^2/(4*tan(pi/k)) using double arithmetic. */
    double nd = (double)n;
    double kd = (double)k;
    double area = kd * nd * nd / (4.0 * tan(PI / kd));

    /* Convert the floating value to an unsigned integer.  Values up to 2^53
       are represented exactly by a double, so truncation gives the floor. */
    unsigned long long val_uint = (unsigned long long)(area);
    return (seq_t)val_uint;
}
