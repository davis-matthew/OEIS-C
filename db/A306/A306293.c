/*
 * A306293  Number of binary words of length n such that in every prefix and
 *          in every suffix the number of 0's and the number of 1's differ by
 *          at most two.
 * https://oeis.org/A306293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: counts binary words of length n for which every
   prefix and every suffix has the number of 0’s and 1’s differ by at
   most two.  For a fixed total imbalance T (the final walk position)
   the walk must stay inside the interval J(T) = [-2,2] ∩ [T-2,T+2].
   The walk has only five possible positions (−2…2), so for each T we
   perform a small DP over these positions.  The total number is the
   sum over all admissible T (parity compatible with n and |T|≤2). */

#include <stdbool.h>

enum { ST = 5 };   /* mapping position s in [−2,2] to index s+2 */

seq_t a(idx_t n)
{
    /* The empty word is the only word of length 0. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)0;
    const int T_values[ST] = { -2, -1, 0, 1, 2 };

    for (int ti = 0; ti < ST; ++ti) {
        int T = T_values[ti];

        /* T must have the same parity as n. */
        if ((T & 1) != (int)(n & 1))
            continue;

        /* Compute the intersection J(T) = [−2,2] ∩ [T−2,T+2] */
        int L = (T <= 0) ? -2 : T - 2;
        int U = (T >= 0) ?  2 : T + 2;

        /* Mark allowed positions inside J(T) */
        bool allowed[ST] = { false };
        for (int s = L; s <= U; ++s)
            allowed[s + 2] = true;

        /* DP arrays for current and next step */
        seq_t cur[ST] = { 0 };
        seq_t nxt[ST] = { 0 };

        /* start at difference 0 */
        if (allowed[2])
            cur[2] = (seq_t)1;

        for (idx_t step = 1; step <= n; ++step) {
            /* reset next array */
            for (int i = 0; i < ST; ++i)
                nxt[i] = (seq_t)0;

            /* transition to allowed positions */
            for (int i = 0; i < ST; ++i) {
                if (!allowed[i])
                    continue;

                /* position corresponds to difference i−2 */
                if (i > 0 && allowed[i - 1])
                    nxt[i] += cur[i - 1];
                if (i + 1 < ST && allowed[i + 1])
                    nxt[i] += cur[i + 1];
            }
            /* swap cur and nxt */
            for (int i = 0; i < ST; ++i)
                cur[i] = nxt[i];
        }

        /* add words ending with total imbalance T */
        result += cur[T + 2];
    }

    return result;
}
