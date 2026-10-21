/*
 * A296859  Numbers whose base-2 digits have #(pits) > #(peaks); see Comments.
 * https://oeis.org/A296859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search over natural numbers. */
/* Sequence A296859: numbers whose base‑2 digits have #(pits) > #(peaks). */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* count of numbers already found that satisfy the condition   */
    idx_t found = 0;
    /* candidate number to test, starting from 1 */
    seq_t num = 1;

    while (found < n)
    {
        /* determine bit length of num */
        int bits = 0;
        seq_t t = num;
        while (t) { t >>= 1; ++bits; }

        /* count pits and peaks only if at least 3 bits exist */
        if (bits >= 3)
        {
            int pits  = 0;
            int peaks = 0;

            /* scan all 3‑bit windows in the binary representation
               of num, where the middle bit is an interior position. */
            for (idx_t j = 0; j <= (idx_t)bits - (idx_t)3; ++j)
            {
                /* extract the three bits starting at position j */
                unsigned int mask = (unsigned int)((num >> j) & 7u);

                /* pit: pattern 101 (decimal 5)
                   peak: pattern 010 (decimal 2) */
                if (mask == 5u)  ++pits;
                else if (mask == 2u) ++peaks;
            }

            if (pits > peaks) ++found;
        }

        ++num;   /* try next integer */
    }

    /* num was incremented after the last matching number was counted,
       so the current value is the required term. */
    return num - 1;
}
