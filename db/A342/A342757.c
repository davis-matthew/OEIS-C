/*
 * A342757  Array read by ascending antidiagonals: T(k, n) is the minimum
 *          value of the magic constant in a perimeter-magic k-gon of order n.
 * https://oeis.org/A342757
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k, ncol) where T(k,n) = ((1-(k mod 2))*(n mod
 *            2)+k*(n^2-2n+2)+n)/2. The pair (k, ncol) is obtained by reading
 *            the 2‑D array T(k,n) (k,n≥3) along ascending antidiagonals with
 *            k decreasing inside each diagonal
 * Range:     u8 n <= 65, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* a(idx_t n) returns the n-th term of OEIS A342757 for any n ≥ 3.
   If n < 3 the function returns 0 (the value is undefined but the
   function never crashes). */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    /* Convert the sequence index into the zero‑based offset along the
       antidiagonal traversal. */
    idx_t offset = n - 3;   /* offset of the first term (k=3,n=3) */

    /* Find the diagonal number t (t = S‑5 where S = k+n is the sum
       value of the diagonal).  The diagonal contains t elements.
       We find the smallest t such that t*(t+1)/2 > offset. */
    idx_t t = 1;
    while ((t * (t + 1) / 2) <= offset) ++t;

    /* The number of elements before this diagonal */
    idx_t prev = (t - 1) * t / 2;

    /* Position within the current diagonal (0 based) */
    idx_t pos = offset - prev;          /* 0 ≤ pos < t */

    /* Coordinates of the term:  ncol increases while k decreases.
       ncol = pos + 3;   k = (t + 2) – pos;   because k ranges from 3 to t+2
       inside the diagonal. */
    idx_t n_col = pos + 3;
    idx_t k_val = (t + 2) - pos;

    /* Compute T(k, ncol) using only integer arithmetic.
       Use 64‑bit intermediates to avoid overflow for all supported sizes. */
    unsigned long long n_ll = (unsigned long long)n_col;
    unsigned long long k_ll = (unsigned long long)k_val;

    /* tmp = ((1-(k mod 2))*(n mod 2) + k*(n^2-2n+2) + n) / 2  */
    unsigned long long tmp = ((unsigned long long)1 - (k_ll & 1ULL))
                           * (n_ll & 1ULL)
                           + k_ll * (n_ll * n_ll - 2ULL * n_ll + 2ULL)
                           + n_ll;
    tmp /= 2ULL;

    return (seq_t)tmp;
}
