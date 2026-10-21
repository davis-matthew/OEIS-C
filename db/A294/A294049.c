/*
 * A294049  Number of binary strings of length n avoiding substrings 1000,
 *          1011, 1101, or 1111.
 * https://oeis.org/A294049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 56, u64 n <= 115,
 *            u128 n <= 234, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294049:
   Number of binary strings of length n avoiding substrings
   1000, 1011, 1101, or 1111.
   The implementation builds a DFA of 8 states (all prefixes of the
   forbidden patterns of length <=3) and counts accepted strings
   of length n by dynamic programming.  The result fits in seq_t
   for all values that fit in the target integer implementation. */
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Build automaton on first call */
    static bool initialized = false;
    static int trans[8][2];
    if (!initialized) {
        /* States: 0:"", 1:"1", 2:"10", 3:"100", 4:"101",
           5:"11", 6:"110", 7:"111" */
        static const char state_strs[8][4] = {
            "",    /* 0 */
            "1",   /* 1 */
            "10",  /* 2 */
            "100", /* 3 */
            "101", /* 4 */
            "11",  /* 5 */
            "110", /* 6 */
            "111"  /* 7 */
        };
        static const size_t state_len[8] = {0,1,2,3,3,2,3,3};
        static const char forb[4][5] = {
            "1000",
            "1011",
            "1101",
            "1111"
        };

        char buf[5];
        for (int s = 0; s < 8; ++s) {
            for (int b = 0; b < 2; ++b) {
                /* Build candidate string = state_strs[s] + b */
                memcpy(buf, state_strs[s], state_len[s]);
                buf[state_len[s]] = '0' + b;
                buf[state_len[s]+1] = '\0';

                /* Check forbidden transition */
                bool forbidden = false;
                if (state_len[s] + 1 == 4) {
                    for (int f = 0; f < 4; ++f) {
                        if (strcmp(buf, forb[f]) == 0) {
                            forbidden = true;
                            break;
                        }
                    }
                }

                if (forbidden) {
                    trans[s][b] = -1;          /* dead state */
                    continue;
                }

                /* Find longest suffix that is a known state prefix */
                int next_state = 0;   /* fallback to empty prefix */
                size_t max_l = (state_len[s] + 1 > 3) ? 3 : state_len[s] + 1;
                for (size_t l = max_l; l > 0; --l) {
                    const char *suffix = buf + (state_len[s] + 1 - l);
                    for (int t = 0; t < 8; ++t) {
                        if (state_len[t] == l &&
                            strncmp(suffix, state_strs[t], l) == 0) {
                            next_state = t;
                            goto found;
                        }
                    }
                }
                found:
                trans[s][b] = next_state;
            }
        }
        initialized = true;
    }

    /* Dynamic programming over the automaton states */
    seq_t cur[8];
    seq_t nxt[8];
    for (int k = 0; k < 8; ++k) cur[k] = (seq_t)0;
    cur[0] = (seq_t)1;   /* empty string of length 0 */

    for (idx_t i = 0; i < n; ++i) {
        for (int k = 0; k < 8; ++k) nxt[k] = (seq_t)0;
        for (int s = 0; s < 8; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            for (int b = 0; b < 2; ++b) {
                int ns = trans[s][b];
                if (ns < 0) continue;    /* forbidden */
                nxt[ns] += val;
            }
        }
        for (int k = 0; k < 8; ++k) cur[k] = nxt[k];
    }

    seq_t total = (seq_t)0;
    for (int k = 0; k < 8; ++k) total += cur[k];
    return total;
}
