/*
 * A340461  a(n) = 2*sigma(phi(n)) - n.
 * https://oeis.org/A340461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * sigma(phi(n)) - n
 * Range:     u8 n <= 60, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* sequence is defined for n >= 1 */

    seq_t phi_n = calc_phi((seq_t)n);           /* φ(n) */
    seq_t sigma_phi = sigma_of((idx_t)phi_n);   /* σ(φ(n)) */
    return 2 * sigma_phi - (seq_t)n;            /* a(n) */
}
