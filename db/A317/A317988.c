/*
 * A317988  Farey fraction tree, reading each fraction's numerator first, then
 *          its denominator.
 * https://oeis.org/A317988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A317988. The sequence lists all reduced fractions k/d (0 ≤ k ≤
 *            d) with 0 ≤ k/d ≤ 1, ordered by increasing denominator d, and
 *            within each denominator by increasing numerator k. Each fraction
 *            contributes two terms: first its numerator, then its
 *            denominator. Thus a(2m−1) is the numerator of the m‑th...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Determine the pair index (1‑based) and whether we want numerator */
    idx_t pair   = (n + 1) / 2;          /* 1‑based pair number   */
    bool need_num = (n % 2) == 1;        /* odd index → numerator */

    /* Enumerate fractions in Farey order until the desired pair */
    idx_t count = 0;                     /* number of pairs seen so far */

    /* Handle denominator 1: fractions 0/1 and 1/1 */
    count++;
    if (count == pair)
        return need_num ? (seq_t)0 : (seq_t)1;

    count++;
    if (count == pair)
        return need_num ? (seq_t)1 : (seq_t)1;

    /* Denominators >= 2 */
    for (idx_t d = 2; ; ++d)
    {
        for (idx_t k = 1; k < d; ++k)
        {
            if (gcd((seq_t)k, (seq_t)d) == (seq_t)1)
            {
                count++;
                if (count == pair)
                    return need_num ? (seq_t)k : (seq_t)d;
            }
        }
    }
}
