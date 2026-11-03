/*
 * A266539  Terms of A006257 (Josephus problem) repeated.
 * https://oeis.org/A266539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=0, a(2)=0; For n ≥ 3: let m = (n-1)/2. Then a(n) = 2 * (m -
 *            2^⌊log₂ m⌋) + 1. This is the Josephus (k=2) sequence J(m)
 *            repeated twice
 * Range:     u8 n <= 768, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9510 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    idx_t m = (n - 1) / 2;
    idx_t k = ilog2(m);
    idx_t power = (idx_t)1 << k;
    idx_t l = m - power;
    return ((seq_t)2 * l) + 1;
}
