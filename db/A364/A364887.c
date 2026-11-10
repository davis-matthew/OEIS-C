/*
 * A364887  Inverse permutation to A364884.
 * https://oeis.org/A364887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A364884(k) = n (inverse permutation);
 *            computed by generating A364884 (T(1,1)=1, T(n,k) = least unseen
 *            multiple of the k-th flattened term) until n appears
 * Range:     u8 n <= 60, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static void A364887_ensure_seq(idx_t *cap, seq_t **arr, idx_t needed)
{
    if (*cap < needed) {
        idx_t nc = (*cap) ? (*cap << 1) : 4;
        while (nc < needed) nc <<= 1;
        *arr = mem_resize(seq_t, *arr, (nc + 1));
        *cap = nc;
    }
}

static void A364887_ensure_pos(idx_t *cap, idx_t **pos, idx_t needed)
{
    if (*cap < needed) {
        idx_t nc = (*cap) ? (*cap << 1) : 128;
        while (nc < needed) nc <<= 1;
        *pos = (idx_t *)realloc(*pos, (size_t)(nc + 1) * sizeof(idx_t));
        memset(*pos + (*cap) + 1, 0, (size_t)(nc - *cap) * sizeof(idx_t));
        *cap = nc;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static bool   inited   = false;
    static idx_t  size     = 0;
    static idx_t  cap_seq  = 0;
    static idx_t  cap_cand = 0;
    static seq_t *a_vals   = NULL;
    static seq_t *cand     = NULL;
    static idx_t  cur_row  = 2;
    static idx_t *pos      = NULL;   /* pos[value] = index in A364884, 0 if unseen */
    static idx_t  pos_cap  = 0;

    if (!inited) {
        cap_seq = cap_cand = 4;
        a_vals = mem_new(seq_t, (cap_seq + 1));
        cand   = mem_new(seq_t, (cap_cand + 1));
        a_vals[1] = (seq_t)1;
        cand[1]   = (seq_t)2;
        pos_cap = 128;
        pos = (idx_t *)calloc((size_t)(pos_cap + 1), sizeof(idx_t));
        pos[1] = 1;
        size = 1;
        inited = true;
    }

    while (!(n <= pos_cap && pos[n] != 0)) {
        A364887_ensure_seq(&cap_seq, &a_vals, size + cur_row);
        A364887_ensure_seq(&cap_cand, &cand, size + cur_row);

        for (idx_t k = 1; k <= cur_row; ++k) {
            seq_t base = a_vals[k];
            seq_t candidate = cand[k];

            for (;;) {
                idx_t ci = (idx_t)candidate;
                if (ci <= pos_cap && pos[ci] != 0) candidate += base;
                else break;
            }

            ++size;
            a_vals[size] = candidate;

            A364887_ensure_pos(&pos_cap, &pos, (idx_t)candidate);
            pos[(size_t)candidate] = size;

            cand[k] = candidate + base;
            cand[size] = candidate * (seq_t)2;
        }
        ++cur_row;
    }

    return (seq_t)pos[n];
}
