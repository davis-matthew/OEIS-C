/*
 * A278640  Number of pairs of orientable necklaces with n beads and up to 4
 *          colors; i.e., turning the necklace over does not leave it
 *          unchanged. The turned-over necklace is not included in the count.
 * https://oeis.org/A278640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(4^floor((n+1)/2)+4^ceil((n+1)/2))/4 + (1/(2n)) *
 *            ∑_{d|n} φ(d) * 4^(n/d) (n>0) a(0)=0, a(1)=0, a(2)=0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   calc_phi, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4.h"     /* pow4(idx_t exp) : returns 4^exp as seq_t   */
#include "../utils/calc_phi.h" /* calc_phi(seq_t m) : Euler's totient of m   */

seq_t a(idx_t n)
{
    /* Handle offset terms */
    if (n == 0 || n == 1 || n == 2)
        return 0;

    /* floor((n+1)/2) and ceil((n+1)/2) */
    idx_t floor_half = (n + 1) / 2;
    idx_t ceil_half  = (n + 2) / 2;

    seq_t pow_floor = pow4(floor_half);
    seq_t pow_ceil  = pow4(ceil_half);

    /* First part of the formula: -(4^floor + 4^ceil)/4 */
    seq_t term1 = (pow_floor + pow_ceil) / 4;

    /* Sum over divisors:  S = Σ_{d|n} φ(d) * 4^(n/d) */
    seq_t S = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* Contribution from divisor d1 */
            seq_t phi1 = calc_phi(d1);
            seq_t pow_d2 = pow4(d2);          /* 4^(n/d1) == 4^d2 */
            S += phi1 * pow_d2;

            if (d1 != d2)                     /* divisor d2, if distinct */
            {
                seq_t phi2 = calc_phi(d2);
                seq_t pow_d1 = pow4(d1);      /* 4^(n/d2) == 4^d1 */
                S += phi2 * pow_d1;
            }
        }
    }

    /* Second part of the formula: (1/(2n)) * S */
    seq_t denom  = ((seq_t)2 * n);
    seq_t S_div  = S / denom;

    /* Final value: S/(2n) - term1  (non‑negative for all n>0) */
    return S_div - term1;
}
