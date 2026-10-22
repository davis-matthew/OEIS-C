/*
 * A329679  Number of excursions of length n with Motzkin-steps consisting
 *          only of consecutive steps UH, UD, HD and DH.
 * https://oeis.org/A329679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = number of excursions of length n with Motzkin-steps
 *            consisting only of consecutive steps UH, UD, HD and DH
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;   /* empty walk */

    /* State indices:
       alt 0..1  : current altitude (never exceeds 1)
       last 0..3 : last step taken (0 = none, 1 = U, 2 = H, 3 = D)
    */
    seq_t cur[2][4] = {0};
    seq_t next[2][4] = {0};

    cur[0][0] = (seq_t)1;   /* start at altitude 0, no previous step */

    for (idx_t step = (idx_t)0; step < n; ++step) {
        /* clear next state */
        for (int a = 0; a < 2; ++a)
            for (int l = 0; l < 4; ++l)
                next[a][l] = (seq_t)0;

        for (int alt = 0; alt < 2; ++alt) {
            for (int last = 0; last < 4; ++last) {
                seq_t cnt = cur[alt][last];
                if (cnt == (seq_t)0) continue;

                for (int ns = 1; ns <= 3; ++ns) {   /* next step (U,H,D) */
                    if (last != 0) {   /* adjacency rule */
                        if (!((last == 1 && (ns == 2 || ns == 3)) ||
                              (last == 2 && ns == 3) ||
                              (last == 3 && ns == 2)))
                            continue;
                    }

                    int newalt = alt;
                    if (ns == 1)      newalt = alt + 1;   /* U */
                    else if (ns == 3) newalt = alt - 1;   /* D */
                    /* H leaves altitude unchanged */

                    if (newalt < 0 || newalt > 1) continue;

                    next[newalt][ns] += cnt;
                }
            }
        }

        /* swap cur and next */
        for (int a = 0; a < 2; ++a)
            for (int l = 0; l < 4; ++l)
                cur[a][l] = next[a][l];
    }

    /* sum all paths ending at altitude 0 (any final step) */
    seq_t result = (seq_t)0;
    for (int last = 1; last <= 3; ++last)
        result += cur[0][last];

    return result;
}
