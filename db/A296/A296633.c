/*
 * A296633  Number of nX6 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A296633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts the number of n×6 binary arrays in which each 1 has 0, 2
 *            or 4 neighbours among its horizontal, vertical and anti‑diagonal
 *            adjacent cells. The sequence satisfies a(1)=21, a(2)=286,
 *            a(3)=3581, …
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>


/* --- Utility: neighbour check for a single row -------------------- */
static inline bool valid_row(uint8_t cur, uint8_t above, uint8_t below,
                             bool has_above, bool has_below)
{
    for (int i = 0; i < 6; ++i) {
        if (!(cur & (1u << i))) continue;          /* cell is 0 → nothing to check */

        int cnt = 0;

        /* horizontal neighbours */
        if (i > 0 && (cur & (1u << (i - 1)))) cnt++;
        if (i < 5 && (cur & (1u << (i + 1)))) cnt++;

        /* neighbours in the row above */
        if (has_above) {
            if (above & (1u << i))                 cnt++;    /* vertical up */
            if (i < 5 && (above & (1u << (i + 1)))) cnt++; /* anti‑diagonal up‑right */
        }

        /* neighbours in the row below */
        if (has_below) {
            if (below & (1u << i))                  cnt++;    /* vertical down */
            if (i > 0 && (below & (1u << (i - 1)))) cnt++; /* anti‑diagonal down‑left */
        }

        if (cnt != 0 && cnt != 2 && cnt != 4) return false;
    }
    return true;
}

/* --- Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* small index handling – not required but harmless */
    if (n == 0) return (seq_t)0;

    /* Pre‑compute all admissible transitions once */
    static bool trans_init = false;
    static unsigned char trans[4096][64];
    static unsigned char trans_cnt[4096];
    if (!trans_init) {
        for (uint8_t above = 0; above < 64; ++above) {
            for (uint8_t curr  = 0; curr  < 64; ++curr) {
                uint16_t id = ((uint16_t)above << 6) | curr;
                unsigned char cnt = 0;
                for (uint8_t next = 0; next < 64; ++next) {
                    if (valid_row(curr, above, next, true, true))
                        trans[id][cnt++] = next;
                }
                trans_cnt[id] = cnt;
            }
        }
        trans_init = true;
    }

    /* n == 1 is a special case – no neighbour rows exist */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t r = 0; r < 64; ++r)
            if (valid_row(r, 0, 0, false, false))
                ans += 1;
        return ans;
    }

    /* DP tables – static to survive multiple calls, but re‑initialised each call */
    static seq_t dp0[4096], dp1[4096];
    seq_t *cur = dp0, *nxt = dp1;

    /* initialise with a single leading “zero” row */
    for (int i = 0; i < 4096; ++i) cur[i] = 0;
    for (uint8_t r = 0; r < 64; ++r) cur[r] = 1;   /* state id: prev=0, curr=r */

    /* iterate over rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear the next DP table */
        for (int i = 0; i < 4096; ++i) nxt[i] = 0;

        /* process all current states */
        for (uint16_t id = 0; id < 4096; ++id) {
            seq_t val = cur[id];
            if (!val) continue;

            unsigned char *list = trans[id];
            unsigned char cnt = trans_cnt[id];
            uint8_t curr = id & 63;                 /* lower 6 bits = current row */

            for (unsigned char k = 0; k < cnt; ++k) {
                uint8_t next = list[k];
                uint16_t nid = ((uint16_t)curr << 6) | next;
                nxt[nid] += val;
            }
        }

        /* swap tables for next iteration */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* final validation – no row below the last one */
    seq_t result = 0;
    for (uint16_t id = 0; id < 4096; ++id) {
        seq_t val = cur[id];
        if (!val) continue;

        uint8_t prev = id >> 6;
        uint8_t curr = id & 63;

        if (valid_row(curr, prev, 0, true, false))
            result += val;
    }

    return result;
}
