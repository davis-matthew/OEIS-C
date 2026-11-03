/*
 * A333385  a(n) = 3^n + 2 * 17^n for n >= 0.
 * https://oeis.org/A333385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2 * 17^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 755 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* compute 3^n and 17^n using the pre‑built power routine */
    seq_t pow3 = ipow_u64_idx(3, n);
    seq_t pow17 = ipow_u64_idx(17, n);

    /* a(n) = 3^n + 2 * 17^n */
    return pow3 + ((seq_t)2 * pow17);
}
