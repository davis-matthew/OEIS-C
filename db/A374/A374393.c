/*
 * A374393  Triangular numbers such that the three numbers before it and the
 *          three numbers after it are squarefree.
 * https://oeis.org/A374393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th triangular number T such that the three integers
 *            * preceding T and the three integers succeeding T are squarefree
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, just return 0 as a guard */

    idx_t count = 0;
    for (idx_t m = 1; ; ++m)
    {
        seq_t T = ((seq_t)m * ((seq_t)m + 1)) / 2;

        /* If the three numbers before T would under‑flow (i.e. become <= 0),
           this candidate cannot satisfy the condition.  */
        if (T <= 3) continue;

        bool ok = true;
        for (int d = -3; d <= 3; ++d)
        {
            if (d == 0) continue;
            if (d < 0)
            {
                seq_t neg = (seq_t)(-d);
                if (T <= neg) { ok = false; break; }
            }

            seq_t t = T + (seq_t)d;
            if (!is_squarefree(t)) { ok = false; break; }
        }

        if (ok)
        {
            ++count;
            if (count == n) return T;
        }
    }
}
