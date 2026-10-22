/*
 * A345916  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum <= 0.
 * https://oeis.org/A345916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n‑th nonnegative integer k whose k‑th
   composition in standard order (graded reverse‑lexicographic)
   has reverse‑alternating sum <= 0. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* count qualifying k's; iterate k from 0 upward */
    idx_t count = 0;
    for (idx_t k = 0;; ++k)
    {
        /* collect gaps between successive 1‑bits (positions are 1‑based) */
        uint64_t diffs[128];   /* maximum possible L is 64 for a 64‑bit number */
        int L = 0;
        uint64_t pos = 1;      /* current bit position (1‑based) */
        uint64_t temp = k;
        int prev = 0;          /* last position where a 1 was found */

        while (temp)
        {
            if (temp & 1ULL)
            {
                int d = (int)(pos - prev);
                diffs[L++] = (uint64_t)d;
                prev = (int)pos;
            }
            temp >>= 1;
            ++pos;
        }

        /* compute alternating sum over the parts (reverse order of diffs) */
        int64_t s = 0;
        int sign = 1;          /* + for the first part in reverse order */
        for (int i = L - 1; i >= 0; --i)
        {
            s += sign * (int64_t)diffs[i];
            sign = -sign;
        }

        /* check the reverse‑alternating sum condition */
        bool ok;
        if ((L & 1) == 0)      /* even L : rev = -s, need s >= 0 */
            ok = (s >= 0);
        else                   /* odd L : rev =  s, need s <= 0 */
            ok = (s <= 0);

        if (ok)
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
