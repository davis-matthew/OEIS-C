/*
 * A362022  a(n) is the least positive integer whose binary expansion is the
 *          concatenation of the binary expansions of two numbers whose
 *          product is n.
 * https://oeis.org/A362022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min_{d|n} concat_bin(d, n/d) ; where concat_bin(a,b) is
 *            the binary concatenation of a and b
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Concatenate the binary expansions of a and b. */
static seq_t concat_bin(idx_t a, idx_t b)
{
    seq_t aa = (seq_t)a;
    seq_t bb = (seq_t)b;
    unsigned len_b = bitlen_seq_t(bb);  /* number of bits in b */
    return (aa << len_b) | bb;
}

seq_t a(idx_t n)
{
    bool first = true;      /* flag for first candidate */
    seq_t best = 0;         /* will be set on first candidate */

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;

            seq_t val1 = concat_bin(d, e);
            if (first || val1 < best)
            {
                best = val1;
                first = false;
            }

            if (d != e)
            {
                seq_t val2 = concat_bin(e, d);
                if (val2 < best)
                {
                    best = val2;
                }
            }
        }
    }
    return best;
}
