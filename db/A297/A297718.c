/*
 * A297718  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A297718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   middle_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297718: Number of n×6 binary arrays with each 1 having 1, 2 or 4 neighboring 1s  
   (horizontally, diagonally or anti‑diagonally adjacent).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/middle_ok.h"

static bool   valid[64][64][64];
static uint8_t next_list[4096][64];
static uint8_t next_count[4096];
static bool   initialized = false;

static void init_matrices(void) {
    if (initialized) return;
    for (int a = 0; a < 64; ++a) {
        for (int m = 0; m < 64; ++m) {
            for (int b = 0; b < 64; ++b) {
                valid[a][m][b] = middle_ok((uint8_t)a, (uint8_t)m, (uint8_t)b);
            }
        }
    }
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            int state = (prev << 6) | curr;
            uint8_t l = 0;
            for (int nxt = 0; nxt < 64; ++nxt) {
                if (valid[prev][curr][nxt]) next_list[state][l++] = (uint8_t)nxt;
            }
            next_count[state] = l;
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    init_matrices();
    if (n == 0) return 0;

    seq_t answer = 0;

    if (n == 1) {
        for (int r = 0; r < 64; ++r) {
            if (valid[0][r][0]) ++answer;
        }
        return answer;
    }

    static seq_t dp_cur[4096];
    static seq_t dp_next[4096];

    memset(dp_cur, 0, sizeof(dp_cur));
    for (int r = 0; r < 64; ++r) dp_cur[r] = 1; /* state: prev=0, curr=r */

    for (idx_t step = 1; step < n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int state = 0; state < 4096; ++state) {
            seq_t ways = dp_cur[state];
            if (ways == 0) continue;
            uint8_t curr = (uint8_t)(state & 0x3F);
            uint8_t use = next_count[state];
            uint8_t *plist = next_list[state];
            for (uint8_t k = 0; k < use; ++k) {
                uint8_t nxt = plist[k];
                int ns = (curr << 6) | nxt;
                dp_next[ns] += ways;
            }
        }
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    for (int state = 0; state < 4096; ++state) {
        seq_t ways = dp_cur[state];
        if (ways == 0) continue;
        uint8_t prev = (uint8_t)(state >> 6);
        uint8_t curr = (uint8_t)(state & 0x3F);
        if (valid[prev][curr][0]) answer += ways;
    }

    return answer;
}
