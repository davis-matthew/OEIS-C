/*
 * A396061  Numbers k such that k < reverse(k) and reverse(k^2) =
 *          reverse(k)^2.
 * https://oeis.org/A396061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   rev, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute-force enumeration: find k with decimal digits from {0,1,2,3} such that
   k < reverse(k) and reverse(k^2) == reverse(k)^2.  The function scans
   all admissible numbers in increasing order, counting those that satisfy
   the conditions until the n-th term is reached. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/rev.h"      /* rev(idx_t x) */
#include "../utils/reverse.h"  /* reverse(seq_t v) */

extern seq_t a(idx_t n)
{
    idx_t count = 0;
    const int MAX_LEN = 25;                 /* safe upper bound for small n */
    uint8_t digits[MAX_LEN];

    for (int len = 2; ; ++len)              /* numbers have at least 2 digits */
    {
        digits[0] = 1;                      /* leading digit cannot be 0 */
        for (int i = 1; i < len; ++i) digits[i] = 0;

        while (1)
        {
            /* build the current number */
            idx_t k = 0;
            for (int i = 0; i < len; ++i)
                k = k * 10 + digits[i];

            /* apply the defining conditions */
            if (k < rev(k))
            {
                seq_t ksquared = (seq_t)k * (seq_t)k;
                seq_t rev_ksq  = reverse(ksquared);
                seq_t rev_k    = rev(k);
                seq_t rev_k_sq = rev_k * rev_k;
                if (rev_ksq == rev_k_sq)
                {
                    ++count;
                    if (count == n)
                        return (seq_t)k;
                }
            }

            /* increment to the next number with allowed digits */
            int pos = len - 1;
            while (pos > 0)
            {
                if (++digits[pos] < 4)
                    break;
                digits[pos] = 0;
                --pos;
            }
            if (pos == 0)           /* carry from the most significant digit */
            {
                if (digits[0] == 3)  /* last digit of this length exhausted */
                    break;           /* move to next length */
                digits[0]++;         /* 1→2 or 2→3 */
            }
        }
    }
}
