/*
 * A381659  Index where n first appears in A381658.
 * https://oeis.org/A381659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least index m such that A381658(m) = n; A381658 is built
 *            by greedy search (earliest term keeping at most one common
 *            difference k per value t with t = a(i) = a(i+k) = a(i+2k))
 * Range:     u8 n <= 10, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct A381659_info {
    idx_t *pos;
    size_t len;
    size_t cap;
    bool has_triple;
};
static struct A381659_info *A381659_infos = NULL;
static size_t A381659_infos_cap = 0;
static idx_t A381659_gen = 0;

static void A381659_ensure(size_t val)
{
    if (val < A381659_infos_cap)
        return;
    size_t nc = A381659_infos_cap ? A381659_infos_cap : 128;
    while (nc <= val)
        nc <<= 1;
    A381659_infos = (struct A381659_info *)realloc(A381659_infos, nc * sizeof(struct A381659_info));
    for (size_t i = A381659_infos_cap; i < nc; ++i) {
        A381659_infos[i].pos = NULL;
        A381659_infos[i].len = 0;
        A381659_infos[i].cap = 0;
        A381659_infos[i].has_triple = false;
    }
    A381659_infos_cap = nc;
}

static void A381659_add(size_t val, idx_t pos)
{
    struct A381659_info *info = &A381659_infos[val];
    if (info->len == info->cap) {
        size_t nc = info->cap ? (info->cap << 1) : 8;
        info->pos = (idx_t *)realloc(info->pos, nc * sizeof(idx_t));
        info->cap = nc;
    }
    info->pos[info->len++] = pos;
}

static bool A381659_bsearch(const idx_t *a, size_t len, idx_t target)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < len && a[lo] == target);
}

static bool A381659_can_place(size_t val, idx_t pos, bool *made_triple)
{
    struct A381659_info *info = &A381659_infos[val];
    *made_triple = false;
    if (info->len == 0)
        return true;
    int found = 0;
    for (size_t idx = 0; idx < info->len; ++idx) {
        idx_t j = info->pos[idx];
        if (pos <= j)
            continue;
        if (2 * j <= pos)
            continue;
        idx_t i = 2 * j - pos;
        if (i == 0 || i >= j)
            continue;
        if (A381659_bsearch(info->pos, idx, i)) {
            if (info->has_triple)
                return false;
            if (found)
                return false;
            found = 1;
        }
    }
    *made_triple = (found != 0);
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    A381659_ensure((size_t)n);
    while (A381659_infos[(size_t)n].len == 0) {
        idx_t next_idx = A381659_gen + 1;
        size_t cand = 1;
        for (;;) {
            A381659_ensure(cand);
            bool t = false;
            if (A381659_can_place(cand, next_idx, &t)) {
                A381659_add(cand, next_idx);
                if (t)
                    A381659_infos[cand].has_triple = true;
                break;
            }
            ++cand;
        }
        A381659_gen = next_idx;
    }
    return (seq_t)A381659_infos[(size_t)n].pos[0];
}
