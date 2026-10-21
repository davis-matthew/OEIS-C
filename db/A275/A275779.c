/*
 * A275779  a(n) = (2^(n^2) - 1)/(1 - 1/2^n).
 * https://oeis.org/A275779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} 2^(k*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of the OEIS sequence A275779.
 *
 * a(n) = (2^(n^2) - 1) / (1 - 1/2^n)
 *      = (2^(n^2) - 1) * 2^n / (2^n - 1)
 *      = Sum_{k=1..n} 2^(k*n)
 *
 * The function works for any seq_t type provided by the build:
 * unsigned integer of 8,16,32,64,128 bits, or an arbitrary‑precision
 * integer class (C++17).  It never relies on floating point.
 * For non‑fitting results in fixed‑width types the usual overflow
 * behaviour applies, which is acceptable because the tests only
 * check values that fit in seq_t.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n < 1, but safe guard */

    /* Compute 2^(n^2) and 2^n */
    const idx_t n_squared = n * n;
    const seq_t pow_n  = pow2(n);
    const seq_t pow_nn = pow2(n_squared);

    /* (2^(n^2) - 1) * 2^n */
    seq_t numerator = pow_nn - 1;
    numerator = numerator * pow_n;

    /* (2^n - 1) */
    const seq_t denominator = pow_n - 1;

    /* Result */
    return numerator / denominator;
}
