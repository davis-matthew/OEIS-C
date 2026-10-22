/*
 * A345032  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} k^(floor(n/j) - 1).
 * https://oeis.org/A345032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A345032 : Square array T(n,k) read by antidiagonals downwards.
 *  For n ≥ 1 the sequence a(n) is obtained by reading the array
 *
 *      T(n,k) = Σ_{j=1..n} k^(floor(n/j)-1)      (n ≥ 1, k ≥ 0)
 *
 *  along antidiagonals { (n,k) | n+k-1 = d } in order of increasing d,
 *  and, for each d, increasing n (i.e. from the top to the bottom of
 *  the diagonal).  Hence a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), ... .
 *
 *  The implementation below translates an index n into the corresponding
 *  pair (n_row,k) and evaluates T(n_row,k) directly.  The calculation
 *  is efficient for n up to ~10⁴ (and even beyond, limited only by the
 *  size of seq_t).  No external libraries are required besides the
 *  standard C headers.                                                */

#include <stdlib.h>

/* Estimate the diagonal index d for a given sequence index n.
 * d is the smallest integer with (d+1)(d+2)/2 ≥ n.  */
static idx_t find_diagonal(idx_t n)
{
    idx_t d = 0;
    /* loop until the triangular number reaches or exceeds n */
    while (((d + 1) * (d + 2)) / 2 < n)
        ++d;
    return d;
}

/* Main entry function for sequence A345032.  */
seq_t a(idx_t n)
{
    /* 1. Locate the diagonal and the position within it. */
    idx_t d   = find_diagonal(n);                 /* diagonal index   */
    idx_t prev = d * (d + 1) / 2;                 /* numbers before d */
    idx_t pos  = n - prev - 1;                    /* 0‑based offset  */
    idx_t n_row = pos + 1;                         /* row index in T   */
    idx_t k_val  = d - pos;                        /* column index     */

    /* 2. Evaluate T(n_row,k_val).  */
    if (k_val == 0) {                                      /* special case 0^0 = 1 */
        /* Floor(n/j) = 1  ⇔  j > n/2.  Those are the j that give 1 instead of 0. */
        return (seq_t)(n_row - n_row / 2);
    }

    /* pre‑compute powers k^e for e = 0 .. n_row-1  (n_row is small). */
    seq_t *pow = (seq_t *)malloc((size_t)n_row * sizeof(seq_t));
    if (!pow) return 0;                     /* graceful failure if out of memory */

    pow[0] = 1;
    seq_t base = (seq_t)k_val;
    for (idx_t e = 1; e < n_row; ++e) {
        pow[e] = pow[e - 1] * base;
    }

    /* sum over j = 1 .. n_row of k^(floor(n_row/j)-1) */
    seq_t sum = 0;
    for (idx_t j = 1; j <= n_row; ++j) {
        idx_t exp = n_row / j - 1;          /* floor(n_row/j) - 1 ≥ 0 */
        sum += pow[exp];
    }

    free(pow);
    return sum;
}
