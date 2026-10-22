/*
 * A320199  Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 11*d^2 = n.
 * https://oeis.org/A320199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 11*d^2 = n */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static caches for the triple‑sum counts and the a‑term counts */
    static seq_t *cnt   = NULL;    /* cnt[s] = number of (d,c,b) with sum = s */
    static idx_t  cnt_sz = 0;
    static seq_t *aCnt   = NULL;    /* aCnt[t] = 1 if t==0, 2 if t>0 and perfect square, else 0 */
    static idx_t  aCnt_sz = 0;

    /* ensure the caches cover up to n */
    if (cnt_sz <= n || aCnt_sz <= n) {
        idx_t new_cap = n;
        /* resize and recompute cnt array */
        seq_t *new_cnt = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        memset(new_cnt, 0, (new_cap + 1) * sizeof(seq_t));

        /* enumerate all integer triples (d,c,b) up to new_cap */
        int dmax = 0;
        while ((idx_t)(dmax + 1) * (dmax + 1) * 11 <= new_cap) ++dmax;

        for (int d = -dmax; d <= dmax; ++d) {
            idx_t d2 = (idx_t)(d * d);
            idx_t Sd = 11 * d2;
            if (Sd > new_cap) continue;

            int cmax = 0;
            while ((Sd + 4 * ((idx_t)(cmax + 1) * (cmax + 1))) <= new_cap) ++cmax;

            for (int c = -cmax; c <= cmax; ++c) {
                idx_t c2 = (idx_t)(c * c);
                idx_t Sdc = Sd + 4 * c2;
                if (Sdc > new_cap) continue;

                int bmax = 0;
                while ((Sdc + 2 * ((idx_t)(bmax + 1) * (bmax + 1))) <= new_cap) ++bmax;

                for (int b = -bmax; b <= bmax; ++b) {
                    idx_t b2 = (idx_t)(b * b);
                    idx_t S = Sdc + 2 * b2;
                    if (S <= new_cap) ++new_cnt[S];
                }
            }
        }

        /* resize and recompute aCnt array */
        seq_t *new_aCnt = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
        if (!new_aCnt) {
            free(new_cnt);
            return 0; /* memory failure – unlikely in contest environment */
        }
        new_aCnt[0] = 1;
        for (idx_t i = 1; ; ++i) {
            idx_t sq = i * i;
            if (sq > new_cap) break;
            new_aCnt[sq] = 2;
        }

        /* free old caches if any */
        if (cnt) free(cnt);
        if (aCnt) free(aCnt);

        cnt   = new_cnt;
        aCnt  = new_aCnt;
        cnt_sz = new_cap + 1;
        aCnt_sz = new_cap + 1;
    }

    /* compute the answer for this n */
    seq_t ans = 0;
    for (idx_t s = 0; s <= n; ++s) {
        if (!cnt[s]) continue;
        idx_t t = n - s;
        if (!aCnt[t]) continue;
        ans += cnt[s] * aCnt[t];
    }
    return ans;
}
