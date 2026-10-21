/*
 * A293578  Triangular array read by rows. One form of sieve of Eratosthenes
 *          (see comments for construction).
 * https://oeis.org/A293578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_isqrt_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A293578: Triangular array read by rows (sieve of Eratosthenes) */
/* The value at flat index n is obtained from its row coordinate r and
   offset t by solving k^2 + t*k - r = 0.  If the equation has an integer
   positive solution k, the value is z = r/k + k - 1; otherwise 0. */

#include <stdbool.h>
#include "../utils/floor_isqrt_u64.h"   /* floor_isqrt_u64 */
#include "../utils/floor_sqrt.h"       /* floor_sqrt for seq_t if needed */

seq_t a(idx_t n)
{
    /* r = row index: smallest r with r^2 >= n */
    idx_t r = floor_isqrt_u64(n - 1) + 1;

    /* number of elements before this row */
    idx_t left = (r - 1) * (r - 1);

    /* zero‑based offset inside the row (0 .. 2r-2) */
    idx_t offset = n - left - 1;

    /* t ranges from -(r-1) to +(r-1) */
    int64_t t = (int64_t)offset - (int64_t)(r - 1);

    /* |t| for discriminant calculation */
    idx_t t_abs = (t >= 0) ? (idx_t)t : (idx_t)(-t);

    /* Discriminant of k^2 + t*k - r = 0: D = t^2 + 4r  */
    idx_t D = t_abs * t_abs + (idx_t)4 * r;

    /* Integer square root of D */
    idx_t s = floor_isqrt_u64(D);
    if (s * s != D) {
        return (seq_t)0;  /* discriminant not a perfect square */
    }

    /* Compute k = (-t + s)/2 with proper sign handling */
    idx_t k;
    if (t >= 0) {
        k = (s - (idx_t)t) / 2;
    } else {
        k = (s + (idx_t)(-t)) / 2;
    }
    if (k == 0) {
        return (seq_t)0;  /* should not happen for positive r */
    }

    /* z = r/k + k - 1  (division exact because k | r) */
    idx_t z_val = (r / k) + k - 1;

    return (seq_t)z_val;
}
