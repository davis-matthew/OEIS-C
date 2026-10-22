/*
 * A399246  Rectangular array read by descending antidiagonals: r(n, k) =
 *          floor(k*x^n), where x = 1+sqrt(2).
 * https://oeis.org/A399246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = floor(k*(1+sqrt{2})^i) where the pair (i,k) is chosen by
 *            reading the array r(i,k)=floor(k*(1+sqrt{2})^i) in descending
 *            antidiagonals. The sequence is indexed starting with a(1)
 * Range:     u8 n <= 7, u16 n <= 26, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Find the diagonal d (1‑based) such that d(d+1)/2 >= n. */
static inline idx_t triangular_root(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__uint128_t)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety guard, real offset starts at 1 */

    /* 1. Find the diagonal and position within it */
    idx_t d          = triangular_root(n);               /* diagonal index */
    idx_t termsPrev  = (d - 1) * d / 2;                   /* T_{d-1} */
    idx_t posDiag    = n - termsPrev;                     /* 1‑based offset in the diagonal */

    /* 2. Array coordinates */
    idx_t row = posDiag;                 /* exponent i (≥1) */
    idx_t col = d + 1 - posDiag;         /* column k (≥1) */

    /* 3. Compute the Pell numbers a=row, b=row for (1+√2)^row  */
    seq_t a_val = 1;   /* a_0 */
    seq_t b_val = 0;   /* b_0 */
    for (idx_t i = 1; i <= row; ++i) {
        seq_t new_a = a_val + (seq_t)2 * b_val;
        seq_t new_b = a_val + b_val;
        a_val = new_a;
        b_val = new_b;
    }

    /* 4. Compute floor(k*b_val*√2) as floor(√(2*(k*b_val)^2)) */
    seq_t k    = (seq_t)col;
    seq_t m    = k * b_val;             /* m = k*b_val */
    seq_t m_sq = m * m;                 /* m² */
    seq_t two_m_sq = m_sq * (seq_t)2;   /* 2*m² */
    seq_t part = floor_sqrt(two_m_sq); /* floor(k*b_val*√2) */

    /* 5. Final value: k*a_val + part */
    seq_t result = k * a_val + part;
    return result;
}
