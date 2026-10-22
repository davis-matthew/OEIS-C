/*
 * A362277  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..floor(n/2)} (-k/2)^j *
 *          binomial(n-j,j)/(n-j)!.
 * https://oeis.org/A362277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 49, i16 n <= 86, i32 n <= 202, i64 n <= 455,
 *            i128 n <= 1117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is obtained by reading the square array T(n,k),
 *   where T(n,k) satisfies the recurrence
 *     T(0,k) = 1,
 *     T(1,k) = 1,
 *     T(n,k) = T(n-1,k) - k*(n-1)*T(n-2,k)   for n > 1.
 *   The array is read by antidiagonals downwards: first T(0,0),
 *   then T(1,0),T(0,1), then T(2,0),T(1,1),T(0,2), ...
 *
 * Implementation uses only integer arithmetic and a short loop
 * to locate the appropriate antidiagonal.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the value a(n) of OEIS A362277. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal d that contains index n.
     * The d-th antidiagonal (starting from d = 0) has length d+1
     * and starts at index d*(d+1)/2.  We find the largest d
     * such that d*(d+1)/2 <= n.
     */
    uint64_t m = (uint64_t)n;
    uint64_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= m)
        d++;

    uint64_t pre = d * (d + 1) / 2;   /* start index of diagonal d */
    uint64_t offset = m - pre;        /* position within the diagonal (0-based) */
    uint64_t j = d - offset;          /* column index k */
    uint64_t i = offset;              /* row index n of T(i,j) */

    /* Compute T(i,j) by the recurrence. */
    if (i == 0 || i == 1)
        return (seq_t)1;                 /* T(0,k)=T(1,k)=1 */

    seq_t t_nm2 = 1;                    /* T(0,j)   */
    seq_t t_nm1 = 1;                    /* T(1,j)   */
    seq_t t_n = 1;                      /* placeholder */

    for (uint64_t n_i = 2; n_i <= i; ++n_i) {
        t_n = t_nm1 - (seq_t)j * (seq_t)(n_i - 1) * t_nm2;
        t_nm2 = t_nm1;
        t_nm1 = t_n;
    }
    return t_nm1;
}
