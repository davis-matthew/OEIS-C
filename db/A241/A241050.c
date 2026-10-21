/*
 * A241050  Number of n X 2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or zero plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays over {0,1,2,3} satisfying the local
 *            mod‑4 constraints described in A241050. It is computed by
 *            dynamic programming with 256 states (column sums modulo 4 and
 *            previous row values)
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 69, u64 n <= 141,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* not strictly needed, but harmless */

seq_t a(idx_t n)
{
    /* state: s0,s1,prev0,prev1 each ∈{0,1,2,3} – 8 bits ⇒ 256 states */
    seq_t dp_cur[256] = {0};
    seq_t dp_next[256];
    dp_cur[0] = 1;            /* start: sums 0, previous row values 0 */

    for (idx_t row = 0; row < n; ++row)
    {
        for (int k = 0; k < 256; ++k) dp_next[k] = 0;

        for (int st = 0; st < 256; ++st)
        {
            seq_t cnt = dp_cur[st];
            if (cnt == 0) continue;

            int s0   = (st >> 6) & 3;   /* sum above column 0 mod 4 */
            int s1   = (st >> 4) & 3;   /* sum above column 1 mod 4 */
            int prev0 = (st >> 2) & 3;  /* a[row-1][0] (if row>0) */
            int prev1 = st & 3;         /* a[row-1][1] (if row>0) */

            for (int v0 = 0; v0 < 4; ++v0)
            {
                /* column 0 constraints */
                if (v0 == 1)                      /* 1 + leftSum (0)        */
                    continue;
                if (v0 == 0)                      /* 0 + sumNW (0)          */
                    continue;
                if (v0 == s0)                     /* 0 + sumAbove (s0)      */
                    continue;
                if (v0 == ((1 + prev1) & 3))      /* 1 + sumAntidiagNE (prev1) */
                    continue;

                for (int v1 = 0; v1 < 4; ++v1)
                {
                    /* column 1 constraints */
                    if (v1 == ((1 + v0) & 3))      /* 1 + leftSum (v0)       */
                        continue;
                    if (v1 == s1)                 /* 0 + sumAbove (s1)      */
                        continue;
                    if (v1 == prev0)              /* 0 + sumNW (prev0)      */
                        continue;
                    if (v1 == 1)                  /* 1 + sumAntidiagNE (0)  */
                        continue;

                    int ns0 = (s0 + v0) & 3;
                    int ns1 = (s1 + v1) & 3;
                    int nst = (ns0 << 6) | (ns1 << 4) | (v0 << 2) | v1;
                    dp_next[nst] += cnt;
                }
            }
        }

        for (int k = 0; k < 256; ++k) dp_cur[k] = dp_next[k];
    }

    seq_t result = 0;
    for (int k = 0; k < 256; ++k) result += dp_cur[k];
    return result;
}
