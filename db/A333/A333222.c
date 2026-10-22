/*
 * A333222  Numbers k such that every restriction of the k-th composition in
 *          standard order to a subinterval has a different sum.
 * https://oeis.org/A333222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333222 – numbers k such that every contiguous sub‑interval of
   the k‑th composition in standard order has a different sum.
   Implementation: scan k from 0 upwards, test the property
   and return the n‑th such k.                                         */

#include <stdint.h>
#include <stdbool.h>

/* Test whether the k‑th composition in standard order has all
   distinct contiguous sub‑interval sums. */
static bool good_k(idx_t k)
{
    /* Collect positions of 1 bits (LSB is position 1). */
    int pos[64];
    int pos_cnt = 0;
    idx_t tmp = k;
    int bit_pos = 1;
    while (tmp) {
        if (tmp & (idx_t)1) {
            pos[pos_cnt++] = bit_pos;
        }
        tmp >>= 1;
        ++bit_pos;
    }

    /* Empty composition is good. */
    if (pos_cnt == 0) return true;

    /* Build parts from the cumulative positions. */
    int parts[64];
    parts[0] = pos[0];
    for (int i = 1; i < pos_cnt; ++i)
        parts[i] = pos[i] - pos[i-1];
    int len = pos_cnt;

    /* Check all contiguous sub‑interval sums for uniqueness. */
    bool used[65] = { false };          /* sums 0..64 */
    for (int i = 0; i < len; ++i) {
        int s = 0;
        for (int j = i; j < len; ++j) {
            s += parts[j];             /* current sub‑interval sum */
            if (used[s]) return false;
            used[s] = true;
        }
    }
    return true;
}

/* Return the n‑th term of A333222 for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not required but harmless */

    idx_t found = 0;
    idx_t k = 0;
    while (true) {
        if (good_k(k)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
        ++k;
    }
}
