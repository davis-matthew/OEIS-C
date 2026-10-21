/*
 * A278430  Number of nX2 0..3 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly one mistake.
 * https://oeis.org/A278430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 36, u128 n <= 201,
 *            big any n
 * Helpers:   init_constants, leq_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A278430 – number of n×2 arrays over {0..3} with rows and columns in
    lexicographic nondecreasing order but with exactly one mistake.
    Computation is performed by dynamic programming that counts
    (a) arrays with rows sorted and columns unsorted (A > B lex);
    (b) arrays with columns sorted and exactly one row violation.
    The answer is the sum of these two counts.
*/
#include <stdbool.h>
#include "../utils/init_constants.h"
#include "../utils/leq_pair.h"

static void compute_counts(idx_t n, seq_t *ca, seq_t *cb)
{
    static bool init = false;
    static bool adj[16][16];
    static int a_val[16], b_val[16];

    if (!init) {
        init = true;
        for (int i = 0; i < 16; ++i) {
            a_val[i] = i / 4;
            b_val[i] = i % 4;
            for (int j = 0; j < 16; ++j)
                adj[i][j] = leq_pair(i, j);
        }
    }

    /* ---- Case A : rows sorted, columns unsorted (A > B lex) ---- */
    seq_t dpA[16][3] = {0}; /* states: 0=eq,1=less,2=greater */
    for (int r = 0; r < 16; ++r) {
        int state = (a_val[r] < b_val[r]) ? 1 :
                    (a_val[r] == b_val[r]) ? 0 : 2;
        dpA[r][state] = 1;
    }

    seq_t nextA[16][3];
    for (idx_t len = 1; len < n; ++len) {
        /* zero next */
        for (int i = 0; i < 16; ++i)
            for (int s = 0; s < 3; ++s) nextA[i][s] = 0;

        for (int p = 0; p < 16; ++p) {
            for (int s = 0; s < 3; ++s) {
                seq_t val = dpA[p][s];
                if (val == 0) continue;
                for (int q = 0; q < 16; ++q) {
                    if (!adj[p][q]) continue;            /* row order */
                    int state = s;
                    int a = a_val[q], b = b_val[q];
                    if (s == 0) {                        /* eq so far */
                        if (a == b) state = 0;
                        else if (a < b) state = 1;
                        else state = 2;                   /* a > b */
                    } /* else keep state */
                    nextA[q][state] += val;
                }
            }
        }
        for (int i = 0; i < 16; ++i)
            for (int s = 0; s < 3; ++s) dpA[i][s] = nextA[i][s];
    }
    seq_t resA = 0;
    for (int r = 0; r < 16; ++r) resA += dpA[r][2]; /* greater */

    /* ---- Case B : columns sorted, exactly one row mistake ---- */
    seq_t dpB[16][2][2] = {0}; /* colState:0=eq,1=less ; usedMistake:0/1 */
    for (int r = 0; r < 16; ++r) {
        int state = (a_val[r] < b_val[r]) ? 1 :
                    (a_val[r] == b_val[r]) ? 0 : 2; /* 2 is greater */
        if (state == 2) continue;                  /* columns must be sorted */
        dpB[r][state][0] = 1;
    }

    seq_t nextB[16][2][2];
    for (idx_t len = 1; len < n; ++len) {
        for (int i = 0; i < 16; ++i)
            for (int s = 0; s < 2; ++s)
                for (int m = 0; m < 2; ++m)
                    nextB[i][s][m] = 0;

        for (int p = 0; p < 16; ++p) {
            for (int cs = 0; cs < 2; ++cs) {
                for (int used = 0; used < 2; ++used) {
                    seq_t val = dpB[p][cs][used];
                    if (val == 0) continue;
                    for (int q = 0; q < 16; ++q) {
                        /* row order: allow one violation */
                        bool prev_leq = adj[p][q];
                        int new_used = used;
                        if (!prev_leq) {
                            if (used) continue;         /* second violation forbidden */
                            new_used = 1;
                        }

                        /* column state must stay in {eq, less} */
                        int a = a_val[q], b = b_val[q];
                        int new_cs = cs;
                        if (cs == 0) {
                            if (a == b) new_cs = 0;
                            else if (a < b) new_cs = 1;
                            else continue;                 /* would be greater – reject */
                        } /* if cs==1 keep it */

                        nextB[q][new_cs][new_used] += val;
                    }
                }
            }
        }
        for (int i = 0; i < 16; ++i)
            for (int s = 0; s < 2; ++s)
                for (int m = 0; m < 2; ++m)
                    dpB[i][s][m] = nextB[i][s][m];
    }

    seq_t resB = 0;
    for (int r = 0; r < 16; ++r)
        for (int cs = 0; cs < 2; ++cs)
            resB += dpB[r][cs][1];   /* exactly one row mistake */

    *ca = resA;
    *cb = resB;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t ca, cb;
    compute_counts(n, &ca, &cb);
    return ca + cb;
}
