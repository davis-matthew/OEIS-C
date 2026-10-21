/*
 * A299045  Rectangular array: A(n,k) = Sum_{j=0..k}
 *          (-1)^floor(j/2)*binomial(k-floor((j+1)/2), floor(j/2))*(-n)^(k-j),
 *          n >= 1, k >= 0, read by antidiagonals.
 * https://oeis.org/A299045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth term read by antidiagonals from the array A(n,
 *            k) which satisfies A(n,0) = 1, A(n,1) = 1 – n, A(n,k) =
 *            –n·A(n,k‑1) – A(n,k‑2) (k ≥ 2). For a given sequence index `idx`
 *            (1‑based) we find the diagonal s with s·(s+1)/2 ≥ idx, then
 *            offset k = idx – s·(s‑1)/2 – 1, and row r = s – k. Th...
 * Range:     i8 n <= 32, i16 n <= 72, i32 n <= 181, i64 n <= 453,
 *            i128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the diagonal number s such that s*(s+1)/2 >= idx.
   Uses only integer arithmetic. */
static idx_t find_diagonal(idx_t idx)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* Grow `high` until its triangular number exceeds idx. */
    while (1) {
        unsigned __int128 t = (unsigned __int128)high * (high + 1) / 2;
        if (t >= idx)
            break;
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search in [low, high] for the smallest s with T(s) >= idx. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= idx)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t idx)
{
    if (idx < 1)
        return (seq_t)0;  // undefined for idx < 1

    /* Locate diagonal s and offset k. */
    idx_t s = find_diagonal(idx);
    unsigned __int128 prev_tri = (unsigned __int128)s * (s - 1) / 2;
    idx_t prev_tri_idx    = (idx_t)prev_tri;           /* < idx <= UINT64_MAX */
    idx_t k = idx - prev_tri_idx - 1;                  /* k = offset on diagonal */
    idx_t r = s - k;                                   /* row number n */

    seq_t nrow = (seq_t)r;   /* convert row number to seq_t for arithmetic */

    /* Compute a_{r,k} using the recurrence. */
    if (k == 0)
        return (seq_t)1;
    if (k == 1)
        return (seq_t)1 - nrow;

    seq_t a_prev2 = (seq_t)1;           /* A(r,0) */
    seq_t a_prev1 = (seq_t)1 - nrow;    /* A(r,1) */

    for (idx_t i = 2; i <= k; ++i) {
        seq_t tmp = nrow * a_prev1;       /* n * a_{k-1} */
        seq_t cur = -tmp - a_prev2;       /* -n*a_{k-1} - a_{k-2} */
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
