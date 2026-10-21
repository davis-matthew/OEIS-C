/*
 * A253031  Number of (n+2)X(3+2) 0..2 arrays with every consecutive three
 *          elements in every row and column having exactly two distinct
 *          values, and new values 0 upwards introduced in row major order.
 * https://oeis.org/A253031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A253031 by dynamic programming over
   admissible row patterns. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAXPAT 243   /* 3^5 possible 5‑element rows over {0,1,2} */
#define MAXTRANS 32  /* maximal number of rows that can follow a pair of rows */

static bool initialized = false;

/* Tables for legal rows and transitions */
static int legalRows[MAXPAT];
static int legalCount;
static int patternToIdx[MAXPAT];
static int rowDigits[MAXPAT][5];
static bool rowHas1[MAXPAT];
static bool rowOkSeen0[MAXPAT];
static bool rowStartsZero[MAXPAT];
static int transCount[MAXPAT][MAXPAT];
static int transList[MAXPAT][MAXPAT][MAXTRANS];

/* Vertical rule: in any column, a triple must not be all equal or all
   distinct. */
static inline bool vertical_ok(int a, int b, int c) {
    bool all_equal   = (a == b && b == c);
    bool all_distinct = (a != b && a != c && b != c);
    return !(all_equal || all_distinct);
}

/* Build tables once */
static void init(void) {
    if (initialized) return;
    initialized = true;

    memset(patternToIdx, -1, sizeof(patternToIdx));

    for (int pat = 0; pat < MAXPAT; ++pat) {
        int tmp = pat;
        for (int k = 0; k < 5; ++k) {
            rowDigits[pat][k] = tmp % 3;
            tmp /= 3;
        }

        /* Check horizontal constraint for all 3 consecutive triples */
        bool ok = true;
        for (int j = 0; j < 3; ++j) {
            int a = rowDigits[pat][j];
            int b = rowDigits[pat][j + 1];
            int c = rowDigits[pat][j + 2];
            bool all_equal    = (a == b && b == c);
            bool all_distinct = (a != b && a != c && b != c);
            if (all_equal || all_distinct) { ok = false; break; }
        }
        if (!ok) continue;

        /* Record legal row */
        patternToIdx[pat] = legalCount;
        legalRows[legalCount] = pat;

        /* Properties of the row -------- */
        int firstPos1 = 5, firstPos2 = 5;
        bool has1 = false;
        for (int k = 0; k < 5; ++k) {
            if (rowDigits[pat][k] == 1) {
                has1 = true;
                if (firstPos1 == 5) firstPos1 = k;
            }
            if (rowDigits[pat][k] == 2 && firstPos2 == 5) firstPos2 = k;
        }
        rowHas1[legalCount] = has1;
        rowStartsZero[legalCount] = (rowDigits[pat][0] == 0);
        /* Pattern is acceptable when 2 does not precede 1 */
        rowOkSeen0[legalCount] = (firstPos2 == 5) || (firstPos1 < firstPos2);

        ++legalCount;
    }

    /* Precompute transitions for every pair of rows */
    for (int a = 0; a < legalCount; ++a) {
        for (int b = 0; b < legalCount; ++b) {
            int cnt = 0;
            for (int c = 0; c < legalCount; ++c) {
                bool ok = true;
                for (int col = 0; col < 5; ++col) {
                    if (!vertical_ok(rowDigits[legalRows[a]][col],
                                     rowDigits[legalRows[b]][col],
                                     rowDigits[legalRows[c]][col])) {
                        ok = false; break;
                    }
                }
                if (ok) {
                    transList[a][b][cnt++] = c;
                }
            }
            transCount[a][b] = cnt;
        }
    }
}

/* Sequence function */
seq_t a(idx_t n) {
    init();           /* Ensure precomputation */

    if (n == 0) return (seq_t)0;   /* offset is 1 */

    idx_t totalRows = n + 2;        /* number of rows in the array */

    static seq_t dp[2][MAXPAT][MAXPAT];
    static seq_t dp_next[2][MAXPAT][MAXPAT];

    /* Zero DP tables */
    for (int s = 0; s < 2; ++s)
        for (int i = 0; i < legalCount; ++i)
            for (int j = 0; j < legalCount; ++j)
                dp[s][i][j] = (seq_t)0;

    /* Initial two rows */
    for (int i = 0; i < legalCount; ++i) {          /* first row */
        if (!rowStartsZero[i]) continue;
        if (!rowOkSeen0[i]) continue;

        int seenAfter = rowHas1[i] ? 1 : 0;
        for (int j = 0; j < legalCount; ++j) {      /* second row */
            if (seenAfter == 0 && !rowOkSeen0[j]) continue;
            int seenNow = seenAfter | (rowHas1[j] ? 1 : 0);
            dp[seenNow][i][j] += (seq_t)1;
        }
    }

    /* Iterate over remaining rows */
    idx_t curRows = 2;
    while (curRows < totalRows) {
        /* clear next table */
        for (int s = 0; s < 2; ++s)
            for (int i = 0; i < legalCount; ++i)
                for (int j = 0; j < legalCount; ++j)
                    dp_next[s][i][j] = (seq_t)0;

        for (int seen = 0; seen < 2; ++seen) {
            for (int prev2 = 0; prev2 < legalCount; ++prev2) {
                for (int prev1 = 0; prev1 < legalCount; ++prev1) {
                    seq_t val = dp[seen][prev2][prev1];
                    if (val == (seq_t)0) continue;

                    int tcnt = transCount[prev2][prev1];
                    for (int t = 0; t < tcnt; ++t) {
                        int curr = transList[prev2][prev1][t];
                        if (seen == 0 && !rowOkSeen0[curr]) continue;
                        int newSeen = seen | (rowHas1[curr] ? 1 : 0);
                        dp_next[newSeen][prev1][curr] += val;
                    }
                }
            }
        }

        /* copy back */
        for (int s = 0; s < 2; ++s)
            for (int i = 0; i < legalCount; ++i)
                for (int j = 0; j < legalCount; ++j)
                    dp[s][i][j] = dp_next[s][i][j];

        ++curRows;
    }

    /* sum up all states */
    seq_t answer = (seq_t)0;
    for (int seen = 0; seen < 2; ++seen)
        for (int i = 0; i < legalCount; ++i)
            for (int j = 0; j < legalCount; ++j)
                answer += dp[seen][i][j];

    return answer;
}
