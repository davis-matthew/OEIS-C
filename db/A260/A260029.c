/*
 * A260029  Convert the last term from decimal to binary! a(1)=9.
 * https://oeis.org/A260029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=9; for n>1, a(n) is the decimal number whose digits are *
 *            the binary representation of a(n‑1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not used, just safety */

    /* Base case */
    if (n == 1) return (seq_t)9;

    seq_t val = (seq_t)9;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (seq_t)0;
        unsigned bits = bitlen_seq_t(val);

        /* Process bits from most significant to least significant. */
        for (unsigned j = 0; j < bits; ++j)
        {
            seq_t bit = (val >> (bits - 1 - j)) & (seq_t)1;
            next = next * (seq_t)10 + bit;
        }

        val = next;            /* a(i) */
    }
    return val;
}
