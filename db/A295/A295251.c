/*
 * A295251  Number of n X 6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 3 1's.
 * https://oeis.org/A295251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0‑1 arrays in which every 1 has * 0, 1, or
 *            3 adjacent 1’s horizontally or vertically. * * Implementation:
 *            dynamic programming over rows. States are pairs * (prev, upto)
 *            where * prev – bitmask of the previous row (6 bits) * upto –
 *            bitmask of positions in the previous row that...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COL        6
#define STATE_CNT  (64 * 64)   /* 4096 */

/* -----  static tables  ----- */
static bool      _initialized = false;
static short     _state_id[64][64];
static unsigned char _trans_len[STATE_CNT];
static short     _trans_next[STATE_CNT][64];
static bool      _valid_end[STATE_CNT];
static unsigned char _state_prev[STATE_CNT];
static unsigned char _state_upto[STATE_CNT];
static unsigned char _hnb[64][COL];

/* -----  transition pre‑computation  ----- */
static void _init_tables(void)
{
    /* Map (prev, upto) → state ID */
    short sid = 0;
    for (int prev = 0; prev < 64; ++prev)
        for (int upto = 0; upto < 64; ++upto) {
            _state_id[prev][upto]   = sid;
            _state_prev[sid]        = (unsigned char)prev;
            _state_upto[sid]        = (unsigned char)upto;
            ++sid;
        }

    /* Pre‑compute horizontal neighbour counts for each mask */
    for (int mask = 0; mask < 64; ++mask)
        for (int j = 0; j < COL; ++j) {
            unsigned cnt = 0;
            if (j > 0 && (mask & (1 << (j-1)))) ++cnt;
            if (j+1 < COL && (mask & (1 << (j+1)))) ++cnt;
            _hnb[mask][j] = (unsigned char)cnt;
        }

    /* Build transitions and validity of terminal states */
    for (int prev = 0; prev < 64; ++prev)
        for (int upto = 0; upto < 64; ++upto) {
            int cur_id = _state_id[prev][upto];
            unsigned len = 0;

            /* Iterate over all possible next rows */
            for (int cur = 0; cur < 64; ++cur) {
                bool ok = true;
                for (int j = 0; j < COL && ok; ++j) {
                    if (!(prev & (1 << j))) continue;   /* no 1 in prev at j */
                    int total = _hnb[prev][j]
                              + ((upto & (1 << j)) ? 1 : 0)
                              + ((cur  & (1 << j)) ? 1 : 0);
                    if (!(total == 0 || total == 1 || total == 3))
                        ok = false;
                }
                if (ok) {
                    int next_id = _state_id[cur][prev];
                    _trans_next[cur_id][len++] = (short)next_id;
                }
            }
            _trans_len[cur_id] = (unsigned char)len;

            /* Valid end: check that the last row can have a 0 below */
            bool end_ok = true;
            for (int j = 0; j < COL && end_ok; ++j) {
                if (!(prev & (1 << j))) continue;
                int total = _hnb[prev][j]
                          + ((upto & (1 << j)) ? 1 : 0);   /* down = 0 */
                if (!(total == 0 || total == 1 || total == 3))
                    end_ok = false;
            }
            _valid_end[cur_id] = end_ok;
        }
}

/* -----  main sequence function  ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (!_initialized) {
        _init_tables();
        _initialized = true;
    }

    /* DP arrays */
    seq_t dp[STATE_CNT];
    seq_t next[STATE_CNT];
    seq_t zero = 0;
    for (int i = 0; i < STATE_CNT; ++i) {
        dp[i] = zero;
        next[i] = zero;
    }

    /* start state: previous row all 0, and no above neighbours */
    int init_id = _state_id[0][0];
    dp[init_id] = (seq_t)1;

    /* row by row DP */
    for (idx_t row = 0; row < n; ++row) {
        /* clear next */
        for (int i = 0; i < STATE_CNT; ++i)
            next[i] = zero;

        for (int sid = 0; sid < STATE_CNT; ++sid) {
            seq_t cnt = dp[sid];
            if (cnt == zero)
                continue;
            unsigned tlen = _trans_len[sid];
            for (unsigned t = 0; t < tlen; ++t) {
                int nid = _trans_next[sid][t];
                next[nid] += cnt;
            }
        }
        /* copy next → dp for next iteration */
        for (int i = 0; i < STATE_CNT; ++i)
            dp[i] = next[i];
    }

    /* sum counts of valid terminal states */
    seq_t result = zero;
    for (int sid = 0; sid < STATE_CNT; ++sid)
        if (_valid_end[sid])
            result += dp[sid];

    return result;
}
