/*
 * A361148  a(n) = phi(n)^4.
 * https://oeis.org/A361148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(n)^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the helper functions from the utilities. */
#include "../utils/calc_phi.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* phi(n) is defined for seq_t; cast n appropriately. */
    seq_t ph = calc_phi((seq_t)n);
    /* Compute phi(n) raised to the 4th power. */
    return seq_pow(ph, 4);
}
