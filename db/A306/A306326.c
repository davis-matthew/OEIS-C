/*
 * A306326  The q-analogs T(q; n,k) of the rascal-triangle, here q = 2.
 * https://oeis.org/A306326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(2; r, k) where * r = floor((sqrt(8*n + 1) - 1) / 2), *
 *            k = n - r*(r + 1)/2, * T(2; r, k) = 1 + (2^k - 1)(2^(r - k) - 1)
 *            (row‑major enumeration of the triangle)
 * Range:     u8 n <= 45, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square‑root for 64‑bit values */
static inline idx_t isqrt_u64(idx_t n)
{
    /* initial approximation using double sqrt */
    idx_t x = (idx_t) sqrt((double) n);
    /* correct the approximation */
    while ((x + 1) * (x + 1) <= n) ++x;
    while (x * x > n) --x;
    return x;
}

/* Compute 2^exp using binary exponentiation */
static inline seq_t pow_two(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the row r */
    idx_t m = 8ULL * n + 1;          /* 8*n + 1 */
    idx_t s = isqrt_u64(m);
    idx_t r = (s - 1) / 2;           /* row index */

    /* Column within the row */
    idx_t offset = r * (r + 1) / 2;  /* terms preceding this row */
    idx_t k = n - offset;            /* column index */

    /* Evaluate the triangle entry */
    seq_t pow_k   = pow_two(k);
    seq_t pow_rk  = pow_two(r - k);
    seq_t term    = (pow_k - 1) * (pow_rk - 1);
    return 1 + term;
}
