/*
 * A334076  a(n) = bitwise NOR of n and 2n.
 * https://oeis.org/A334076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = bitwise NOR of n and 2*n, computed as ~(n | (n << 1))
 *            masked to the bit‑length of 2n
 * Range:     u8 n <= 511, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   _bitlen_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_bitlen_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sn = (seq_t)n;
    seq_t sm = sn | (sn << 1);          /* n OR 2n               */
    int    len = _bitlen_seq(sn) + 1;   /* bit‑length of 2n      */
    seq_t  mask;

    if (len >= (int)(sizeof(seq_t) * 8))
        mask = (seq_t)(-1);             /* all bits 1           */
    else
        mask = ((seq_t)1 << len) - 1;   /* mask = 2^len - 1     */

    return (~sm) & mask;                /* bitwise NOR, masked  */
}
