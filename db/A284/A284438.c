/*
 * A284438  a(n) = phi(n)^n.
 * https://oeis.org/A284438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 192 (2026-11-01)
 * Helpers:   ipow_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t phi_n = totient(n);
    return ipow_idx(phi_n, n);
}
