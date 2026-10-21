/*
 * A249955  Number of length n+5 0..3 arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 arrays over alphabet {0,1,2,3} that
 *            avoid all 6‑term windows where the maximum of any two terms
 *            equals the minimum of the remaining four terms. The code uses
 *            dynamic programming over the last five digits (states 0…1023). A
 *            precomputed bitmap identifies the forbidden 6‑...
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 38, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* precompute which 6‑digit windows are forbidden */
static bool bad_window[4096];
static bool bad_init = false;
static void init_bad(void) {
    if (bad_init) return;
    for (int code = 0; code < 4096; ++code) {
        int vals[6];
        int tmp = code;
        for (int k = 5; k >= 0; --k) {
            vals[k] = tmp & 3;
            tmp >>= 2;
        }
        bool bad = false;
        for (int i = 0; i < 6 && !bad; ++i)
            for (int j = i + 1; j < 6; ++j) {
                int maxij = vals[i] > vals[j] ? vals[i] : vals[j];
                int min_others = 3; /* max value is 3 */
                for (int k = 0; k < 6; ++k)
                    if (k != i && k != j && vals[k] < min_others) min_others = vals[k];
                if (maxij == min_others) { bad = true; break; }
            }
        bad_window[code] = bad;
    }
    bad_init = true;
}

/* cached sequence values: aLen[L] = a(L−5) = a(n) for L = n+5 */
static seq_t *aLen = NULL;
static size_t aLenSize = 0;          /* allocated size in elements */
static size_t computedUpTo = 0;      /* maximum L that has been computed */

/* main sequence function */
seq_t a(idx_t n) {
    /* n is 1-based offset; length of arrays L = n + 5 */
    idx_t L = n + 5;

    init_bad();

    if (L > computedUpTo) {
        size_t newSize = L + 1;           /* indices 0 … L */
        seq_t *tmp = (seq_t*)realloc(aLen, newSize * sizeof(seq_t));
        if (!tmp) {                      /* allocation failed */
            return 0;
        }
        aLen = tmp;
        aLenSize = newSize;

        /* DP arrays: state space is 4^5 = 1024  (last 5 digits) */
        static seq_t dp[1024];
        static seq_t newdp[1024];
        memset(dp, 0, sizeof(dp));
        dp[0] = 1;                       /* empty sequence */
        aLen[0] = 1;                     /* length 0 count */

        for (size_t len = 0; len < newSize - 1; ++len) {
            memset(newdp, 0, sizeof(newdp));
            for (int mask = 0; mask < 1024; ++mask) {
                seq_t cur = dp[mask];
                if (cur == 0) continue;
                for (int d = 0; d < 4; ++d) {
                    if (len >= 5) {                    /* a full 6‑window forms */
                        int win = (mask << 2) | d;     /* 6‑digit code */
                        if (bad_window[win]) continue; /* forbidden */
                    }
                    int newMask = ((mask << 2) & 0x3FF) | d;
                    newdp[newMask] += cur;
                }
            }
            /* sum over all states --> number of valid sequences of length len+1 */
            seq_t sum = 0;
            for (int mask = 0; mask < 1024; ++mask) sum += newdp[mask];
            aLen[len + 1] = sum;
            /* copy newdp into dp for next iteration */
            memcpy(dp, newdp, sizeof(dp));
        }
        computedUpTo = L;
    }
    return aLen[L];
}
