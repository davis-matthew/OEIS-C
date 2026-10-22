/*
 * A335485  Numbers k such that the k-th composition in standard order
 *          (A066099) is not weakly decreasing.
 * https://oeis.org/A335485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n‑th integer k such that the k‑th composition in standard order
   (A066099) is NOT weakly decreasing (i.e., contains a rise).  Calculated
   by iterating k, generating the composition from the reversed binary
   representation, testing for a rise, and collecting the first n such k. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t result = 0;

    for (idx_t k = 1; ; ++k)
    {
        /* Build the composition parts by scanning bits of k from LSB upward. */
        idx_t tmp = k;
        idx_t pos = 1;          /* current bit position (1‑based) */
        idx_t prevpos = 0;      /* position of last 1‑bit, start 0 */
        idx_t parts[64];        /* maximum number of 1‑bits in k when k < 2^64 */
        idx_t cnt = 0;          /* number of parts generated */

        while (tmp > 0)
        {
            if (tmp & 1ULL)
            {
                parts[cnt++] = pos - prevpos;
                prevpos = pos;
            }
            pos++;
            tmp >>= 1;
        }

        /* Test whether the composition (obtained by reversing 'parts')
           is NOT weakly decreasing: look for an increase when walking
           from first part to last. */
        bool not_decreasing = false;
        if (cnt > 1)
        {
            idx_t prev = parts[cnt - 1];   /* first part after reversal */
            for (idx_t i = cnt - 1; i > 0; --i)
            {
                idx_t curr = parts[i - 1];
                if (prev < curr)           /* found a rise */
                {
                    not_decreasing = true;
                    break;
                }
                prev = curr;
            }
        }

        if (not_decreasing)
        {
            ++count;
            if (count == n)
            {
                result = (seq_t)k;
                break;
            }
        }
    }
    return result;
}
