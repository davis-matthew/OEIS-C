/*
 * A349593  Square array read by downward diagonals: for n >= 0, k >= 1,
 *          T(n,k) is the period of {binomial(N,n) mod k: N in Z}.
 * https://oeis.org/A349593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the period T(r,k) of {binomial(N,r) mod k : N∈ℤ} where
 *            the pair (r,k) is the n-th element in the square array read by
 *            downward diagonals: first the element with r=0,k=1 (diagonal 0),
 *            then the elements of diagonal 1 from top to bottom, then
 *            diagonal 2, etc. Thus if d = ⌊(√(8⋅n+1)-1)/2⌋ (at...
 * Range:     u8 n <= 113, u16 n <= 1509, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Validated: standard, pass, 94 terms through n = 4653 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdbool.h>

/* compute largest exponent e such that p^e ≤ nRow */
static inline idx_t log_floor(idx_t nRow, idx_t p) {
    if (nRow < p) return 0;
    idx_t e = 0;
    idx_t val = p;
    while (val <= nRow) {
        ++e;
        /* avoid overflow */
        if (val > nRow / p) break;
        val *= p;
    }
    return e;
}

/* compute T(r,k) according to the multiplicative formula */
static seq_t period_T(idx_t r, idx_t k) {
    if (r == 0) return (seq_t)1;
    seq_t result = (seq_t)k;          /* start with k itself */
    idx_t tmp = k;
    /* factorisation of k by trial division */
    for (idx_t p = 2; p*p <= tmp; ++p) {
        if (tmp % p != 0) continue;
        /* remove all copies of p from tmp */
        while (tmp % p == 0) tmp /= p;
        /* compute additional exponent */
        idx_t add = log_floor(r, p);
        if (add) {
            seq_t factor = 1;
            for (idx_t i = 0; i < add; ++i) factor *= (seq_t)p;
            result *= factor;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        idx_t p = tmp;
        idx_t add = log_floor(r, p);
        if (add) {
            seq_t factor = 1;
            for (idx_t i = 0; i < add; ++i) factor *= (seq_t)p;
            result *= factor;
        }
    }
    return result;
}

/* auxiliary: compute integer square root (floor) */
static inline idx_t int_isqrt(idx_t x) {
    idx_t r = 0;
    while ((r+1)*(r+1) <= x) ++r;
    return r;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    /* find diagonal d and offset within it */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum + (d+1) <= n) {
        cum += d+1;
        ++d;
    }
    idx_t offset = n - cum;           /* 0 ≤ offset ≤ d */

    /* coordinates (row, column) */
    idx_t r = offset;                 /* row index */
    idx_t k = d + 1 - offset;         /* column index (≥1) */

    return period_T(r, k);
}
