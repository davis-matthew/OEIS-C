/*
 * A331787  T(b,n) is the largest m such that there exists N such that none of
 *          S(N), S(N+1), ..., S(N+m-1) is divisible by n, where S(N) is the
 *          sum of digits of N in base b. Square array read by ascending
 *          antidiagonals.
 * https://oeis.org/A331787
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy, tabl
 * Formula:   a(n) enumerates the square array T(b,n) of OEIS A331787 * by
 *            reading it along ascending antidiagonals. * For each index n≥2
 *            the coordinates (b,n0) are recovered from the * diagonal sum
 *            S=b+n0; n0 is only the column index in the row of base b. * The
 *            entry is evaluated by the closed form * * T(b,n0...
 * Range:     u8 n <= 45, u16 n <= 153, u32 n <= 561, u64 n <= 2145,
 *            u128 n <= 7627, big any n
 * Helpers:   diag_from_p, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"
#include "../utils/diag_from_p.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                    /* values below the offset are irrelevant */

    idx_t p = n - 2;                        /* zero‑based position in the sequence */

    /* Determine the diagonal sum S and the offset within that diagonal. */
    idx_t offset;
    idx_t S = diag_from_p(p, &offset);

    /* Recover coordinates: (b,n0) where b≥2, n0≥1. */
    idx_t b   = (S - 1) - offset;
    idx_t n0  = offset + 1;

    /* Compute parameters s and t for the closed form. */
    idx_t base_minus_one = b - 1;
    idx_t s_exp = (n0 - 1) / base_minus_one;                /* exponent for b^s   */
    idx_t t_val = (n0 - 1) % base_minus_one + 1;           /* 1 ≤ t_val ≤ b-1    */

    /* GCD part of the formula. */
    seq_t g = gcd((seq_t)t_val, (seq_t)base_minus_one);

    /* b^s as a seq_t (arbitrary precision or 64‑bit as appropriate). */
    seq_t power = pow_uint((uint64_t)b, s_exp);

    /* Compute the multiplicative factor and the final result. */
    seq_t factor = (seq_t)((seq_t)2 * (seq_t)t_val - g + 1);
    return power * factor - (seq_t)2;
}
