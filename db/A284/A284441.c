/*
 * A284441  Base-3 generalized Pascal triangle P_3 read by rows (see Comments
 *          for precise definition).
 * https://oeis.org/A284441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n+1)-st entry of the base‑3 generalized Pascal *
 *            triangle P_3 read by rows. For the row index r (0‑based) and
 *            column * index p (0‑based) satisfying * n = (r+1)(r+2)/2 + p , 0
 *            ≤ p ≤ r * a(n) equals the number of times the base‑3 string of
 *            index p occurs as * a scattered (subsequence)...
 * Range:     u8 n <= 9479, u16 n <= 9479, u32 n <= 9479, u64 n <= 9479,
 *            u128 n <= 9479, big any n
 * Helpers:   reverse_uint8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include "../utils/reverse_uint8.h"

seq_t a(idx_t n) {
    /* special case for the first entry (row 0) */
    if (n == 0) {
        return (seq_t)1;
    }

    /* determine the row index r such that T(r-1) ≤ n < T(r),
     * where T(r) = (r+1)(r+2)/2.  Use integer arithmetic with
     * a small correction loop; n is at most a few ten thousand,
     * so double precision is perfectly safe. */
    idx_t m = n;
    double d = sqrt((double)8.0 * (double)m + 1.0);
    idx_t r = (idx_t)((d - 3.0) / 2.0);

    /* correct possible off‑by‑one due to rounding */
    while (((double)(r + 1) * (double)(r + 2) / 2.0) <= m) {
        ++r;
    }
    while (r > 0 && ((double)r * (double)(r + 1) / 2.0) > m) {
        --r;
    }

    /* compute column index p */
    idx_t Tprev = r * (r + 1) / 2;   /* T(r-1) */
    idx_t p = n - Tprev;             /* 0 ≤ p ≤ r */

    if (p == 0) {
        /* empty string occurs exactly once */
        return (seq_t)1;
    }

    /* --- build base‑3 digits of row string (n) --- */
    uint8_t d_n[41];
    idx_t tn = r;
    size_t L = 0;
    while (tn > 0) {
        d_n[L++] = (uint8_t)(tn % 3);
        tn /= 3;
    }
    reverse_uint8(d_n, L);

    /* --- build base‑3 digits of column string (k) --- */
    uint8_t d_k[41];
    idx_t tk = p;
    size_t M = 0;
    while (tk > 0) {
        d_k[M++] = (uint8_t)(tk % 3);
        tk /= 3;
    }
    reverse_uint8(d_k, M);

    if (M > L) {
        return (seq_t)0;
    }

    /* --- subsequence counting DP --- */
    seq_t c[42];
    for (size_t i = 0; i <= M; ++i) {
        c[i] = (seq_t)0;
    }
    c[0] = (seq_t)1;

    for (size_t i = 0; i < L; ++i) {
        for (size_t j = M; j > 0; --j) {
            if (d_n[i] == d_k[j - 1]) {
                c[j] += c[j - 1];
            }
        }
    }

    return c[M];
}
