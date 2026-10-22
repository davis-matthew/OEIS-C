/*
 * A344837  Square array T(n, k), n, k >= 0, read by antidiagonals; T(n, k) =
 *          min(n * 2^max(0, w(k)-w(n)), k * 2^max(0, w(n)-w(k))) (where w =
 *          A070939).
 * https://oeis.org/A344837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9087, u16 n <= 9087, u32 n <= 9087, u64 n <= 9087,
 *            u128 n <= 9087, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A344837: a(n) is obtained by reading the square array
   T(n,k) by antidiagonals.  The array is defined by
   T(n,k) = min( n << (max(0, log2(k) - log2(n))),      // shift n
                k << (max(0, log2(n) - log2(k)))  );  // shift k
   with T(0,anything)=0.  Here log2 is floor(log2(v)). */

#include <stdint.h>

/* floor_log2 for idx_t (returns 0 for v=0) */
static inline idx_t floor_log2_idx(idx_t v)
{
    return v == 0 ? 0 : 63 - __builtin_clzll(v);
}

/* Square array entry T(n,k) */
static inline seq_t T(idx_t n, idx_t k)
{
    if (n == 0 || k == 0)
        return 0;

    idx_t w_n = floor_log2_idx(n);
    idx_t w_k = floor_log2_idx(k);

    seq_t t1 = (seq_t)n;
    seq_t t2 = (seq_t)k;

    if (w_k > w_n) {
        t1 <<= (w_k - w_n);
    } else if (w_n > w_k) {
        t2 <<= (w_n - w_k);
    }
    return t1 < t2 ? t1 : t2;
}

/* a(n) : read T(n,k) by antidiagonals */
seq_t a(idx_t n)
{
    /* Find smallest d with (d+1)(d+2)/2 > n */
    idx_t d = 0;
    /* Use 128‑bit integer to avoid overflow in the test */
    while (( (__int128)(d + 1) * (d + 2) / 2 ) <= n)
        ++d;

    /* Index of the first element on diagonal d */
    idx_t before = ( (__int128)d * (d + 1) ) / 2;
    idx_t offset = n - before;    /* 0 <= offset <= d */
    idx_t i = offset;
    idx_t j = d - offset;
    return T(i, j);
}
