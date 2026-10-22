/*
 * A344666  a(n) is the number of preference profiles in the stable marriage
 *          problem with 3 men and 3 women that generate n possible stable
 *          matchings.
 * https://oeis.org/A344666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, fini, full
 * Formula:   computed by exhaustive enumeration of all 6^6 preference
 *            profiles for 3 men and 3 women, counting how many stable
 *            matchings each profile has. a(1) = 34080, a(2) = 11484, a(3) =
 *            1092
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static const int perms[6][3] = {
    {0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}
};

static const int matchings[6][3] = {
    {0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}
};

seq_t a(idx_t n)
{
    static seq_t counts[4] = {0,0,0,0};
    static bool init = false;
    if (!init) {
        /* generate all 6^6 preference profiles */
        const idx_t TOTAL = 46656;          /* 6^6 */
        for (idx_t profile = 0; profile < TOTAL; ++profile) {
            /* decode 6 base-6 digits: first 3 for men, next 3 for women */
            idx_t x = profile;
            int manPref[3][3];
            int womanPref[3][3];
            for (int k = 0; k < 3; ++k) {          /* men */
                int p = x % 6;
                x /= 6;
                const int *perm = perms[p];
                for (int i = 0; i < 3; ++i) manPref[k][i] = perm[i];
            }
            for (int k = 0; k < 3; ++k) {          /* women */
                int p = x % 6;
                x /= 6;
                const int *perm = perms[p];
                for (int i = 0; i < 3; ++i) womanPref[k][i] = perm[i];
            }

            /* rank tables: lower index means higher preference */
            int rankMan[3][3];
            int rankWoman[3][3];
            for (int m = 0; m < 3; ++m)
                for (int i = 0; i < 3; ++i)
                    rankMan[m][ manPref[m][i] ] = i;
            for (int w = 0; w < 3; ++w)
                for (int i = 0; i < 3; ++i)
                    rankWoman[w][ womanPref[w][i] ] = i;

            /* count stable matchings for this profile */
            int stable = 0;
            for (int s = 0; s < 6; ++s) {
                const int *match = matchings[s];
                /* inverse mapping: for each woman, who is matched */
                int matchInv[3];
                for (int m = 0; m < 3; ++m) matchInv[ match[m] ] = m;

                bool ok = true;
                for (int m = 0; m < 3 && ok; ++m) {
                    int curW = match[m];
                    for (int i = 0; i < 3 && ok; ++i) {
                        int w = manPref[m][i];
                        if (rankMan[m][w] < rankMan[m][curW]) {
                            int m2 = matchInv[w];
                            if (rankWoman[w][m] < rankWoman[w][m2])
                                ok = false;
                        }
                    }
                }
                if (ok) ++stable;
            }
            if (stable >= 1 && stable <= 3)
                counts[stable] += 1;
        }
        init = true;
    }
    if (n >= 1 && n <= 3) return counts[(int)n];
    return 0;
}
