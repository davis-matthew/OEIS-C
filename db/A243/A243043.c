/*
 * A243043  Number of length n+3 0..7 arrays with no four elements in a row
 *          with pattern abab (with a!=b) and new values 0..7 introduced in
 *          0..7 order.
 * https://oeis.org/A243043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic programming for arrays of length n+3 over {0..7} with no
 *            consecutive "abab" pattern (a!=b) and where new values are
 *            introduced in increasing order (each new value must be the
 *            current maximum+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n) {
    /* length of the arrays */
    const idx_t L = n + 3;          /* n ≥ 1 maps to length 4,5,... */

    /* Constants and state space */
    const int MAX_VAL = 7;         /* largest allowed symbol */
    const int SENTINEL = 8;        /* indicates “no value” for positions < 3 */
    const int BASE = 9;            /* values 0..7 plus SENTINEL */
    const size_t STATE_COUNT = 8 * BASE * BASE * BASE;  /* 5832 */

    /* Static DP tables, allocated once */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static int allocated = 0;

    if (!allocated) {
        dp_cur  = seq_alloc(STATE_COUNT);
        dp_next = seq_alloc(STATE_COUNT);
        allocated = 1;
    }

    /* Initialize DP for length 1: first symbol is 0 */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
    size_t init_idx =
        (((((size_t)0) * BASE + (size_t)SENTINEL) * BASE + (size_t)SENTINEL) * BASE + (size_t)0);
    dp_cur[init_idx] = (seq_t)1;

    /* Iterate until we reach length L */
    for (idx_t len = 1; len < L; ++len) {
        /* Clear next table */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* Process every state */
        for (int maxv = 0; maxv <= MAX_VAL; ++maxv) {
            for (int a3 = 0; a3 < BASE; ++a3) {
                for (int a2 = 0; a2 < BASE; ++a2) {
                    for (int a1 = 0; a1 < BASE; ++a1) {
                        size_t idx =
                            (((((size_t)maxv) * BASE + (size_t)a3) * BASE + (size_t)a2) * BASE + (size_t)a1);
                        seq_t cur_val = dp_cur[idx];
                        if (!cur_val) continue;

                        /* Allowed next symbols: 0..maxv or maxv+1 (if ≤ 7) */
                        int max_limit = (maxv < MAX_VAL) ? (maxv + 1) : MAX_VAL;
                        for (int v = 0; v <= max_limit; ++v) {
                            /* New maximum after inserting v */
                            int new_max = (v > maxv) ? v : maxv;

                            /* Check forbidden pattern a-b-a-b */
                            if (a3 != SENTINEL && a2 != SENTINEL && a1 != SENTINEL) {
                                if (a3 == a1 && a2 == v && a3 != a2) {
                                    continue;  /* pattern matched, skip */
                                }
                            }

                            /* Shift last three positions */
                            int na3 = a2;
                            int na2 = a1;
                            int na1 = v;

                            size_t nidx =
                                (((((size_t)new_max) * BASE + (size_t)na3) * BASE + (size_t)na2) * BASE + (size_t)na1);
                            dp_next[nidx] += cur_val;
                        }
                    }
                }
            }
        }

        /* Swap tables for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all states of final length */
    seq_t result = 0;
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        result += dp_cur[i];
    }
    return result;
}
