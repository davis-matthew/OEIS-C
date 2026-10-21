/*
 * A288797  Square array a(p,q) = p^2 + q^2 - 2*p - 2*q + 2*gcd(p,q), p >= 1,
 *          q >= 1, read by antidiagonals.
 * https://oeis.org/A288797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) is the n‑th element (1‑based) of the anti‑diagonal
 *            traversal of the square array a(p,q)=p²+q²-2p-2q+2*gcd(p,q)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d containing the n‑th term.
       The cumulative size up to diagonal d is d(d-1)/2. */
    idx_t d = (1 + int_sqrt(1 + 8*n)) / 2;
    if (d*(d-1)/2 < n) ++d;            /* carry single correction step */

    /* Count of terms before diagonal d */
    idx_t prev = (d-1)*(d-2)/2;
    /* Position inside the diagonal (1‑based) */
    idx_t pos = n - prev;              /* 1 ≤ pos ≤ d-1 */
    idx_t p = pos;
    idx_t q = d - pos;

    /* Compute a(p,q) using seq_t arithmetic */
    seq_t p_seq = (seq_t)p;
    seq_t q_seq = (seq_t)q;
    seq_t g = gcd(p_seq, q_seq);
    return p_seq*p_seq + q_seq*q_seq - 2*p_seq - 2*q_seq + 2*g;
}
