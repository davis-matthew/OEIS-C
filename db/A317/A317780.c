/*
 * A317780  Number of binary words of length n containing exactly one
 *          occurrence of the subword 01101.
 * https://oeis.org/A317780
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of binary words of length n that contain exactly one *
 *            occurrence of the subword 01101. The implementation uses a *
 *            deterministic finite automaton with states equal to the current
 *            * matched prefix of the pattern
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A317780_H
#define A317780_H

#include <stdbool.h>  // for bool

/* Pattern 01101 of length 5. */
#define PAT_LEN 5
static const unsigned char pattern[PAT_LEN] = {0, 1, 1, 0, 1};

/* Compute the failure function (KMP "pi") for the pattern.  It is
   independent of the input index and is computed once per process. */
static int pi[PAT_LEN];
static void init_pi(void)
{
    static bool done = false;
    if (done) return;
    pi[0] = 0;
    for (int i = 1; i < PAT_LEN; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j])
            j = pi[j - 1];
        if (pattern[i] == pattern[j]) ++j;
        pi[i] = j;
    }
    done = true;
}

/* Count words of length n that contain exactly one occurrence of 01101. */
seq_t a(idx_t n)
{
    init_pi();

    /* For lengths below the pattern, the count is defined to be 0. */
    if (n < PAT_LEN) return 0;

    /* DP arrays: cur[state][flag] and nxt[state][flag].
     * state: 0..PAT_LEN-1   (matched prefix length)
     * flag : 0 = no occurrence yet, 1 = exactly one occurrence seen */
    seq_t cur[PAT_LEN][2] = {{0}};
    seq_t nxt[PAT_LEN][2] = {{0}};

    cur[0][0] = 1;  /* empty word, state 0, no matches */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero the next layer */
        for (int s = 0; s < PAT_LEN; ++s)
            for (int f = 0; f < 2; ++f)
                nxt[s][f] = 0;

        for (int s = 0; s < PAT_LEN; ++s) {
            for (int f = 0; f < 2; ++f) {
                seq_t ways = cur[s][f];
                if (ways == 0) continue;

                for (int b = 0; b <= 1; ++b) {
                    /* compute the new matched prefix length t */
                    int t = s;
                    while (t > 0 && pattern[t] != b)
                        t = pi[t - 1];
                    if (pattern[t] == b) ++t;  /* matched one more character */

                    if (t == PAT_LEN) {        /* full pattern matched */
                        if (f == 1) continue;  /* would be a second occurrence */
                        int next_state = pi[PAT_LEN - 1];  /* allow overlap */
                        nxt[next_state][1] += ways;
                    } else {
                        /* no new occurrence */
                        nxt[t][f] += ways;
                    }
                }
            }
        }

        /* swap cur and nxt for the next position */
        for (int s = 0; s < PAT_LEN; ++s)
            for (int f = 0; f < 2; ++f)
                cur[s][f] = nxt[s][f];
    }

    /* Sum all paths that ended with exactly one occurrence. */
    seq_t result = 0;
    for (int s = 0; s < PAT_LEN; ++s)
        result += cur[s][1];

    return result;
}

#endif /* A317780_H */
