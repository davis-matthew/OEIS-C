/*
 * A327318  Triangular array read by rows: row n shows the coefficients of
 *          this polynomial of degree n: p(x,n) = 2^(n-1) ((x+r)^n -
 *          (x+s)^n)/(r - s), where r = 1 and s = 1/2.
 * https://oeis.org/A327318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient in the polynomial p(x,n) = 2^n *
 *            ((x+1)^n - (x+½)^n), read row‑wise. For the n‑th term of the
 *            sequence the corresponding row r and position k (0‑based)
 *            satisfy r = smallest integer with r(r+1)/2 ≥ n k = n - r(r-1)/2
 *            - 1 Then a(n) = C(r,k) * (2^r - 2^k)
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

static inline seq_t binom(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* find the row r with triangular number >= n */
    idx_t lo = 1, hi = 1;
    while (hi * (hi + 1) / 2 < n) hi <<= 1;   /* double until big enough */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev - 1;

    /* compute coefficient C(r,k) * (2^r - 2^k) */
    seq_t bin = binom(r, k);
    seq_t pow_r = pow2(r);
    seq_t pow_k = pow2(k);
    seq_t diff = pow_r - pow_k;
    return bin * diff;
}
