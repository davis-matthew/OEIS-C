/*
 * A372119  Numbers k such that the k-th composition in standard order is not
 *          biquanimous.
 * https://oeis.org/A372119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k such that the k-th composition in standard
 *            order is not biquanimous
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not used, offset is 1 */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* Build parts of the composition for k */
        idx_t tmp = k;
        idx_t pos = 1;          /* position index in reversed binary (LSB=1) */
        idx_t prev = 0;
        idx_t S = 0;            /* sum of parts = largest set bit position */
        idx_t parts[64];
        idx_t pc = 0;

        while (tmp)
        {
            if (tmp & 1UL)
            {
                parts[pc++] = pos - prev;
                prev = pos;
                if (pos > S) S = pos;
            }
            pos++;
            tmp >>= 1;
        }
        S = prev;   /* largest set bit position */

        bool biquanimous = false;
        if ((S & 1UL) == 0)
        {
            /* Subset‑sum DP for multiset of parts */
            bool reachable[65] = { false };
            reachable[0] = true;
            int S_int = (int)S;
            for (idx_t i = 0; i < pc; ++i)
            {
                int part = (int)parts[i];
                for (int sum = S_int; sum >= part; --sum)
                {
                    if (reachable[sum - part]) reachable[sum] = true;
                }
            }
            if (reachable[S_int / 2]) biquanimous = true;
        }

        if (!biquanimous)
        {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
