/*
 * A292988  Beatty sequence of the real root of 2*x^5 - 9*x^4 + 13*x^3 -
 *          11*x^2 + 5*x - 1; complement of A292987.
 * https://oeis.org/A292988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * r) where r ≈ 2.75393267425618214080
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Q = 10^12  (denominator of the rational approximation of r) */
/*  R = floor(r * Q) = 2753932674256 */
static const unsigned long long _Q = 1000000000000ULL;
static const unsigned long long _R = 2753932674256ULL;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* behaviour below offset is irrelevant */

    /* use an unsigned 128‑bit intermediate to avoid overflow for large n */
    __uint128_t prod = (__uint128_t)(seq_t)n * (__uint128_t)_R;
    return (seq_t)(prod / _Q);
}
