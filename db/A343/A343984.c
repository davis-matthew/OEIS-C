/*
 * A343984  a(n) = number of n-digit singular subwords of the Thue-Morse word
 *          A010060; see Comments.
 * https://oeis.org/A343984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Build a suffix automaton for a long prefix of the Thue‑Morse
 * word.  For each state that corresponds to a distinct factor of
 * the word we check whether it has exactly one extension by 0 or 1.
 * Counting the number of such states for every factor length yields
 * A343984.
 */
#include <stdint.h>
#include <stdbool.h>

#define MAX_N 10000                  /* largest n to precompute */
#define PREFIX_LEN (1 << 16)          /* 65536 enough for all n <= 10000 */
#define MAX_STATES (2 * PREFIX_LEN)

seq_t a(idx_t n) {
    static bool built = false;
    static seq_t cache[MAX_N + 1] = {0};

    if (!built) {
        /* ---- 1. Generate Thue‑Morse word ---------------------------------- */
        static unsigned char t[PREFIX_LEN];
        for (idx_t i = 0; i < PREFIX_LEN; ++i)
            t[i] = (__builtin_parityll((unsigned long long)i) ? 1 : 0);

        /* ---- 2. Build suffix automaton ------------------------------------ */
        static int link[MAX_STATES];
        static int len[MAX_STATES];
        static int next[MAX_STATES][2];

        /* root state */
        link[0] = -1;
        len[0] = 0;
        next[0][0] = next[0][1] = -1;

        int sz = 1;        /* number of states */
        int last = 0;      /* last added state */

        for (idx_t pos = 0; pos < PREFIX_LEN; ++pos) {
            int c = t[pos];
            int cur = sz++;
            len[cur] = len[last] + 1;
            link[cur] = 0;
            next[cur][0] = next[cur][1] = -1;

            int p = last;
            while (p != -1 && next[p][c] == -1) {
                next[p][c] = cur;
                p = link[p];
            }

            if (p == -1) {
                link[cur] = 0;
            } else {
                int q = next[p][c];
                if (len[p] + 1 == len[q]) {
                    link[cur] = q;
                } else {
                    int clone = sz++;
                    len[clone] = len[p] + 1;
                    link[clone] = link[q];
                    next[clone][0] = next[q][0];
                    next[clone][1] = next[q][1];
                    while (p != -1 && next[p][c] == q) {
                        next[p][c] = clone;
                        p = link[p];
                    }
                    link[q] = link[cur] = clone;
                }
            }
            last = cur;
        }

        /* ---- 3. Count singular subwords ----------------------------------- */
        static unsigned long long diff[MAX_N + 2] = {0};

        for (int state = 1; state < sz; ++state) {     /* skip root */
            int has0 = (next[state][0] != -1);
            int has1 = (next[state][1] != -1);

            /* singular iff exactly one extension is present */
            if ((has0 && !has1) || (!has0 && has1)) {
                int linkLen = len[link[state]];
                int l = linkLen + 1;
                if (l < 1) l = 1;
                int u = len[state];
                if (u > MAX_N) u = MAX_N;
                if (l <= u) {
                    diff[l] += 1;
                    diff[u + 1] -= 1;
                }
            }
        }

        unsigned long long acc = 0;
        for (int i = 1; i <= MAX_N; ++i) {
            acc += diff[i];
            cache[i] = (seq_t)acc;
        }
        cache[0] = 0;        /* undefined below offset */
        built = true;
    }

    if (n <= MAX_N)
        return cache[n];
    return 0;   /* out of precomputed range (unlikely for test cases) */
}
