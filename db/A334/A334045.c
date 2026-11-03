/*
 * A334045  Bitwise NOR of binary representation of n and n-1.
 * https://oeis.org/A334045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = (~(n | (n-1))) & ((1 << bitlen(n | (n-1))) - 1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   bit_length, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1, but keeping a safe guard for completeness */
    if (n == 0) return 0;

    seq_t sn  = (seq_t)n;
    seq_t sn1 = (seq_t)(n - 1);
    seq_t m   = sn | sn1;                    /* bits set in either n or n‑1 */

    unsigned bl = bit_length(m);           /* number of significant bits of m */
    seq_t mask   = pow2(bl) - 1;              /* lower bl bits set to 1 */

    return (~m) & mask;                      /* bitwise NOR of n and n‑1 */
}
