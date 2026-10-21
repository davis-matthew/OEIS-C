/*
 * A268057  Triangle T(n,k), 1<=k<=n, read by rows: T(n,k) = number of
 *          iterations of A048158(n, A048158(n, ... A048158(n, k)...)) to
 *          reach 0.
 * https://oeis.org/A268057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  T(n,k) = # of iterations of k → n%k until 0.
    a(m) is the m‑th entry when the triangle T is read by rows.  */
#include "../utils/int_sqrt.h"

static inline idx_t _row_from_index(idx_t pos)
{
    /* Find the smallest n such that n*(n+1)/2 >= pos. */
    idx_t t = 1 + 8 * pos;          /* floor sqrt of (1+8*pos) */
    idx_t r = int_sqrt(t);
    idx_t n = (r - 1) >> 1;         /* floor of (-1+sqrt)/2 */

    /* Adjust if the triangular number is too small. */
    idx_t tri = n * (n + 1) / 2;
    if (tri < pos)
        ++n;
    return n;
}

seq_t a(idx_t pos)
{
    if (pos == 0)
        return 0;   /* position 0 is not part of the sequence */

    idx_t n = _row_from_index(pos);
    idx_t prev_tri = n * (n - 1) / 2;  /* number of entries before row n */

    idx_t k = pos - prev_tri;          /* column within the row */

    seq_t n_seq = (seq_t)n;
    seq_t k_seq = (seq_t)k;

    seq_t cnt = 0;
    while (k_seq != 0) {
        k_seq = n_seq % k_seq;
        ++cnt;
    }
    return cnt;
}
