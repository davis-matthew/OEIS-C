/*
 * A248328  Square array read by antidiagonals downwards: super Patalan
 *          numbers of order 6.
 * https://oeis.org/A248328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = element read by antidiagonals downwards from the super
 *            Patalan numbers of order 6. The array T(n,k) is defined by
 *            T(0,0) = 1, T(n,k) = T(n-1,k)*(36*n-6)/(n+k), T(n,k) =
 *            T(n,k-1)*(36*k-30)/(n+k). a(n) is T(i,j) where i+j = d and j = d
 *            - (n - d(d+1)/2), with d the unique integer such that d(...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Helper: compute (a * num) / den exactly, reducing by gcds first. */
static seq_t mul_div(seq_t a, seq_t num, seq_t den) {
    seq_t g1 = gcd(num, den);
    num /= g1;
    den /= g1;

    seq_t g2 = gcd(a, den);
    a /= g2;
    den /= g2;          /* den should now be 1 */

    return a * num;     /* exact division guarantees no remainder */
}

/* Compute T(i,j) using the two recurrences. */
static seq_t T_of(idx_t i, idx_t j) {
    seq_t res = 1;              /* T(0,0) */
    /* First compute T(i,0) */
    for (idx_t n = 1; n <= i; ++n) {
        seq_t numer = (seq_t)36 * n - 6;   /* 36*n - 6 */
        seq_t denom = n;                    /* n */
        res = mul_div(res, numer, denom);
    }
    /* Then compute T(i,j) from T(i,0) */
    for (idx_t k = 1; k <= j; ++k) {
        seq_t numer = (seq_t)36 * k - 30;   /* 36*k - 30 */
        seq_t denom = i + k;                /* i + k */
        res = mul_div(res, numer, denom);
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }
    idx_t offset = d * (d + 1) / 2;
    idx_t t = n - offset;          /* position within current antidiagonal */
    idx_t i = t;                   /* row index */
    idx_t j = d - t;               /* column index */
    return T_of(i, j);
}
