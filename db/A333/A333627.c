/*
 * A333627  The a(n)-th composition in standard order is the sequence of
 *          run-lengths of the n-th composition in standard order.
 * https://oeis.org/A333627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the run‑length composition of the n‑th
 *            composition in standard order
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* --------- step 1: composition of index n --------- */
    idx_t positions[64];
    int pos_cnt = 0;
    idx_t t = n;
    int bit = 1;
    while (t) {
        if (t & 1ULL) {
            positions[pos_cnt++] = bit;
        }
        t >>= 1;
        ++bit;
    }

    /* differences of positions (before reversal) */
    idx_t parts[64];
    int part_cnt = 0;
    idx_t prev = 0;
    for (int i = 0; i < pos_cnt; ++i) {
        parts[part_cnt++] = positions[i] - prev;
        prev = positions[i];
    }
    /* reverse to obtain the standard composition */
    for (int i = 0; i < part_cnt / 2; ++i) {
        idx_t tmp = parts[i];
        parts[i] = parts[part_cnt - 1 - i];
        parts[part_cnt - 1 - i] = tmp;
    }

    /* --------- step 2: run‑lengths of that composition --------- */
    idx_t r[64];
    int r_cnt = 0;
    if (part_cnt > 0) {
        idx_t cur = parts[0];
        idx_t cnt = 1;
        for (int i = 1; i < part_cnt; ++i) {
            if (parts[i] == cur) {
                ++cnt;
            } else {
                r[r_cnt++] = cnt;
                cur = parts[i];
                cnt = 1;
            }
        }
        r[r_cnt++] = cnt;
    }

    /* --------- step 3: index of this run‑length composition --------- */
    idx_t k = 0;
    if (r_cnt > 0) {
        idx_t sum = 0;
        for (int i = r_cnt - 1; i >= 0; --i) {
            sum += r[i];
            k |= ((idx_t)1ULL << (sum - 1));
        }
    }

    return (seq_t)k;
}
