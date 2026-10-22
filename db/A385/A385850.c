/*
 * A385850  Triangle read by rows: T(n,k) = denominator((Sum_{i=1..k}
 *          (n-i+1)^2)/(Sum_{i=1..k} i^2)), with 1 <= k <= n.
 * https://oeis.org/A385850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look, tabl
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 9705, u32 n <= 9705, u64 n <= 9705,
 *            u128 n <= 9705, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385850
   Formula: T(n,k) = denominator( (1 - 3*k + 2*k^2 + 6*n - 6*k*n + 6*n^2) /
                                (1 + 3*k + 2*k^2) ),
   Triangle read by rows: a(n) = T(r,k) where r and k are obtained from n. */
#include <stdint.h>

static inline idx_t _gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n is 1‑based. For n < 1 the sequence is undefined; return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Find the row r such that 1 + 2 + ... + r >= n.
       cumulative = r*(r+1)/2 after each iteration.              */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column k (1‑based) within the row */
    idx_t k = n - (r - 1) * r / 2;

    /* Compute numerator and denominator using 128‑bit arithmetic
       to avoid overflow for large indices (reasonable for all
       practical inputs).                                            */
    __uint128_t rr = (__uint128_t)r;
    __uint128_t kk = (__uint128_t)k;

    __uint128_t num = 6*rr*rr + 2*kk*kk + 6*rr + 1 - 3*kk - 6*kk*rr;
    __uint128_t den = 2*kk*kk + 3*kk + 1;

    /* Reduce fraction by gcd */
    idx_t num64 = (idx_t)num;  /* safe for the expected range */
    idx_t den64 = (idx_t)den;
    idx_t g = _gcd_u64(num64, den64);

    return (seq_t)(den64 / g);
}
