/*
 * A276856  First differences of the Beatty sequence A022840 for sqrt(6).
 * https://oeis.org/A276856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(6)) - floor((n-1)*sqrt(6))
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation computes the Beatty sequence differences for sqrt(6) using integer arithmetic. */

#include "../utils/floor_sqrt.h"

/* Helper: compute floor(n * sqrt(6)) as floor(sqrt(6*n*n)) */
static inline seq_t floor_n_sqrt6(idx_t n)
{
    seq_t mul = (seq_t)n * (seq_t)n;   /* n squared (fits for n up to ~1e9 on 64‑bit) */
    mul *= (seq_t)6;                  /* multiply by 6 */
    return floor_sqrt(mul);           /* sqrt floor of the product */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n >= 1, difference of successive Beatty terms of sqrt(6) */
    return floor_n_sqrt6(n) - floor_n_sqrt6(n - 1);
}
