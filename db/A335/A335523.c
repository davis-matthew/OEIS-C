/*
 * A335523  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (2,1,1).
 * https://oeis.org/A335523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑negative integer k such that the k‑th
 *            composition in standard order (A066099) avoids the pattern
 *            (2,1,1)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static int contains_pattern(idx_t k)
{
    /* Build the composition of k according to the description:
       - Find positions (1‑indexed) of 1-bits in the binary representation
         of k (least significant bit is position 1).
       - Compute successive differences (gaps) between consecutive positions,
         including the first gap from position 0 (prepended).
       - Reverse the gaps to obtain the composition parts.          */
    int gaps[65];
    int gaps_len = 0;
    int prev_pos = 0;

    idx_t temp = k;
    int pos = 1;
    while (temp)
    {
        if (temp & 1ULL)
        {
            gaps[gaps_len++] = pos - prev_pos;
            prev_pos = pos;
        }
        pos++;
        temp >>= 1;
    }

    if (gaps_len == 0)   /* k == 0 -> empty composition */
        return 0;

    int parts[65];
    int len = gaps_len;
    for (int i = 0; i < len; ++i)
        parts[i] = gaps[len - 1 - i];

    if (len < 3)
        return 0;       /* cannot contain the forbidden pattern */

    /* Scan for the pattern (2,1,1):
       look for indices i < j < k with parts[i] > parts[j] == parts[k]. */
    int pref_max = -1;   /* maximum before the current index */
    for (int j = 0; j < len; ++j)
    {
        int val = parts[j];
        if (pref_max > val)
        {
            for (int kk = j + 1; kk < len; ++kk)
                if (parts[kk] == val)
                    return 1;   /* pattern found */
        }
        if (parts[j] > pref_max)
            pref_max = parts[j];
    }
    return 0;   /* no pattern */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n==0, but keep harmless */

    idx_t count = 0;
    idx_t k = 0;

    while (1)
    {
        if (!contains_pattern(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
