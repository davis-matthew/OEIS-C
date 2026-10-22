/*
 * A326728  A(n, k) = n*(k - 1)*k/2 - k, square array for n >= 0 and k >= 0
 *          read by ascending antidiagonals.
 * https://oeis.org/A326728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   A(n,k) = n*(k-1)*k/2 - k, sequence a(n) is the flattened array *
 *            read by ascending antidiagonals (sum s = n+k) with k varying
 *            from 0 to s
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

typedef uint64_t idx_t; /* assume idx_t is supplied by build */

seq_t a(idx_t idx)
{
    /* Find the diagonal index s such that
     * T(s-1) <= idx < T(s), where T(s) = (s+1)(s+2)/2. */
    long double est = sqrt((long double)(8.0L * (long double)idx + 1.0L));
    idx_t s = (idx_t)((est - 3.0L) / 2.0L);
    if (est < 3.0L) s = 0;             /* guard against negative cast */

    /* Correct for possible rounding errors */
    while (((unsigned __int128)(s + 1) * (s + 2)) / 2 <= idx) ++s;
    while (s > 0 && (((unsigned __int128)s * (s + 1)) / 2) > idx) --s;

    /* Position inside the diagonal */
    idx_t prev = ((unsigned __int128)s * (s + 1)) / 2;
    idx_t offset = idx - prev;          /* 0 <= offset <= s */
    idx_t k = offset;
    idx_t n_val = s - offset;

    /* Compute A(n_val,k) = n*(k-1)*k/2 - k */
    seq_t kk = (seq_t)k;
    seq_t nn = (seq_t)n_val;
    seq_t term = nn * (((kk - 1) * kk) / 2) - kk;
    return term;
}
