/*
 * A361148  a(n) = phi(n)^4.
 * https://oeis.org/A361148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = phi(n)^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9339 (2026-11-01)
 * Helpers:   ipow_idx, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the helper functions from the utilities. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* phi(n) is defined for seq_t; cast n appropriately. */
    seq_t ph = totient_seq((seq_t)n);
    /* Compute phi(n) raised to the 4th power. */
    return ipow_idx(ph, 4);
}
