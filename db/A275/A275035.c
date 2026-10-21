/*
 * A275035  Number of nX6 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-2) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A275035 (number of n×6 arrays over {0,1,2} with the
   constraints that:
   - each element differs from the element to its left (offset (0,-1)),
   - each element differs from the element two rows above and one column left
     (offset (-1,-2)),
   - each element differs from the element two rows above in the same column
     (offset (-2,0)),
   - the values 0,1,2 must appear in that order when first introduced.
   The implementation uses a DP over rows.  The DP states are:
     (prev1, prev2, seen_one)   where
       prev1, prev2  are the patterns of the two last rows,
       seen_one     is 0 if a 1 has not appeared yet, 1 otherwise.
   The transition masks are pre‑computed so that the inner loops iterate
   only over compatible patterns.  The routine is fully static and works
   for any build providing types seq_t and idx_t.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Number of column‑patterns that satisfy the horizontal adjacency rule.
   Value 96 is the number of 6‑tuples over {0,1,2} with no equal neighbours. */
#define NUM_PAT 96

typedef struct { uint64_t lo, hi; } mask_t;

/* pattern data ----------------------------------------------------------- */
static int pattern[NUM_PAT][6];
static bool  has1[NUM_PAT], has2[NUM_PAT];
static int  first1[NUM_PAT], first2[NUM_PAT];

/* compatibility masks ----------------------------------------------------- */
static mask_t compat_row2[NUM_PAT];                         /* a -> b for the second row */
static mask_t compat_all  [NUM_PAT][NUM_PAT];              /* a,b -> c (row >= 3) */
static mask_t compat_state0[NUM_PAT][NUM_PAT];             /* same as compat_all but
                                                             respecting the
                                                             "seen one" rule */

/* DP tables --------------------------------------------------------------- */
static seq_t dp_cur[NUM_PAT][NUM_PAT][2];
static seq_t dp_next[NUM_PAT][NUM_PAT][2];

/* ------------------------------------------------------------------------ */
static void precompute(void)
{
    /* --------------------------------------------------------------------
       Enumerate all 3^6 patterns that have no equal horizontal neighbours.
       -------------------------------------------------------------------- */
    int patcnt = 0;
    for (int num = 0; num < 729; ++num) {
        int tmp = num;
        int d[6];
        for (int i = 0; i < 6; ++i) { d[5-i] = tmp % 3; tmp /= 3; }

        bool ok = true;
        for (int i = 1; i < 6 && ok; ++i)
            if (d[i] == d[i-1]) ok = false;
        if (!ok) continue;

        for (int i = 0; i < 6; ++i) pattern[patcnt][i] = d[i];

        int f1 = -1, f2 = -1;
        for (int i = 0; i < 6; ++i) {
            if (d[i] == 1 && f1 == -1) f1 = i;
            if (d[i] == 2 && f2 == -1) f2 = i;
        }
        has1[patcnt]   = (f1 != -1);
        has2[patcnt]   = (f2 != -1);
        first1[patcnt] = f1;
        first2[patcnt] = f2;
        ++patcnt;
    }

    /* --------------------------------------------------------------------
       Build masks for compatibility:
       * compat_row2[a]      – patterns b that may follow a on the second row
       * compat_all[a][b]    – patterns c that may follow a, b on subsequent rows
       * compat_state0[a][b] – same as compat_all but with the "seen one" rule
       -------------------------------------------------------------------- */
    for (int a = 0; a < NUM_PAT; ++a) {
        mask_t m = {0,0};
        for (int b = 0; b < NUM_PAT; ++b) {
            bool ok = true;
            for (int i = 2; i < 6 && ok; ++i)
                if (pattern[b][i] == pattern[a][i-2]) ok = false;

            if (ok) {
                if (b < 64) m.lo |= ((uint64_t)1 << b);
                else        m.hi |= ((uint64_t)1 << (b-64));
            }
        }
        compat_row2[a] = m;
    }

    for (int a = 0; a < NUM_PAT; ++a) {
        for (int b = 0; b < NUM_PAT; ++b) {
            mask_t all = {0,0};
            mask_t st0 = {0,0};
            for (int c = 0; c < NUM_PAT; ++c) {
                bool ok = true;
                for (int i = 2; i < 6 && ok; ++i)
                    if (pattern[c][i] == pattern[b][i-2]) ok = false;
                if (!ok) continue;
                for (int i = 0; i < 6 && ok; ++i)
                    if (pattern[c][i] == pattern[a][i]) ok = false;
                if (!ok) continue;

                /* general compatibility */
                if (c < 64) all.lo |= ((uint64_t)1 << c);
                else        all.hi |= ((uint64_t)1 << (c-64));

                /* when 1 has not been seen yet, patterns introducing 2
                   before 1 or with 1 appearing after 2 are forbidden. */
                if ( !(has2[c] && !has1[c]) &&
                     !(has2[c] && has1[c] && first1[c] > first2[c]) ) {
                    if (c < 64) st0.lo |= ((uint64_t)1 << c);
                    else        st0.hi |= ((uint64_t)1 << (c-64));
                }
            }
            compat_all[a][b] = all;
            compat_state0[a][b] = st0;
        }
    }
}

/* ------------------------------------------------------------------------ */
static inline int mask_contains(mask_t m, int idx)
{
    return (idx < 64) ? ((m.lo >> idx) & 1) : ((m.hi >> (idx-64)) & 1);
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* lazy precomputation */
    static bool initialized = false;
    if (!initialized) {
        precompute();
        initialized = true;
    }

    /* ----- row 1 -------------------------------------------------------- */
    seq_t first_counts[NUM_PAT][2];
    for (int i = 0; i < NUM_PAT; ++i)
        first_counts[i][0] = first_counts[i][1] = (seq_t)0;

    for (int p = 0; p < NUM_PAT; ++p) {
        if (pattern[p][0] != 0) continue;                    /* first cell must be 0 */

        /* avoid patterns that introduce 2 before 1 already */
        if (has2[p] && !has1[p]) continue;
        if (has2[p] && has1[p] && first1[p] > first2[p]) continue;

        int s = has1[p] ? 1 : 0;
        first_counts[p][s] += (seq_t)1;
    }

    /* ----- n == 1 ------------------------------------------------------- */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int p = 0; p < NUM_PAT; ++p) {
            ans += first_counts[p][0];
            ans += first_counts[p][1];
        }
        return ans;
    }

    /* ----- initial DP for rows 1 and 2 --------------------------------- */
    for (int a = 0; a < NUM_PAT; ++a)
        for (int b = 0; b < NUM_PAT; ++b) {
            dp_cur[a][b][0] = dp_cur[a][b][1] = (seq_t)0;
        }

    for (int a = 0; a < NUM_PAT; ++a) {
        for (int s0 = 0; s0 <= 1; ++s0) {
            seq_t val = first_counts[a][s0];
            if (val == (seq_t)0) continue;

            /* iterate over patterns b compatible with a on the second row */
            for (int b = 0; b < NUM_PAT; ++b) {
                if (!mask_contains(compat_row2[a], b)) continue;

                if (s0 == 0 && has2[b] && !has1[b]) continue;
                if (s0 == 0 && has2[b] && has1[b] && first1[b] > first2[b]) continue;

                int ns = s0 | (has1[b] ? 1 : 0);
                dp_cur[a][b][ns] += val;
            }
        }
    }

    /* ----- n == 2 ------------------------------------------------------- */
    if (n == 2) {
        seq_t ans = (seq_t)0;
        for (int a = 0; a < NUM_PAT; ++a)
            for (int b = 0; b < NUM_PAT; ++b) {
                ans += dp_cur[a][b][0];
                ans += dp_cur[a][b][1];
            }
        return ans;
    }

    /* ----- rows 3 .. n -------------------------------------------------- */
    for (idx_t row = 3; row <= n; ++row) {

        /* zero dp_next */
        for (int a = 0; a < NUM_PAT; ++a)
            for (int b = 0; b < NUM_PAT; ++b) {
                dp_next[a][b][0] = dp_next[a][b][1] = (seq_t)0;
            }

        for (int a = 0; a < NUM_PAT; ++a)
            for (int b = 0; b < NUM_PAT; ++b) {

                seq_t v0 = dp_cur[a][b][0];
                seq_t v1 = dp_cur[a][b][1];
                if (v0 == (seq_t)0 && v1 == (seq_t)0) continue;

                /* transitions from state 0 */
                if (v0 != (seq_t)0) {
                    for (int c = 0; c < NUM_PAT; ++c) {
                        if (!mask_contains(compat_state0[a][b], c)) continue;
                        int ns = has1[c] ? 1 : 0;
                        dp_next[b][c][ns] += v0;
                    }
                }

                /* transitions from state 1 */
                if (v1 != (seq_t)0) {
                    for (int c = 0; c < NUM_PAT; ++c) {
                        if (!mask_contains(compat_all[a][b], c)) continue;
                        dp_next[b][c][1] += v1;
                    }
                }
            }

        /* swap dp_cur and dp_next for the next iteration */
        for (int a = 0; a < NUM_PAT; ++a)
            for (int b = 0; b < NUM_PAT; ++b) {
                dp_cur[a][b][0] = dp_next[a][b][0];
                dp_cur[a][b][1] = dp_next[a][b][1];
            }
    }

    /* ----- final sum ----------------------------------------------------- */
    seq_t ans = (seq_t)0;
    for (int a = 0; a < NUM_PAT; ++a)
        for (int b = 0; b < NUM_PAT; ++b) {
            ans += dp_cur[a][b][0];
            ans += dp_cur[a][b][1];
        }
    return ans;
}
