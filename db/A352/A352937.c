/*
 * A352937  Indices of records in A121216.
 * https://oeis.org/A352937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record (new maximum) in A121216; found
 *            by direct search over A121216, with a(1)=1, a(2)=2 and a(k) =
 *            smallest unused positive integer coprime to a(k-2)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9891 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static size_t R_seq_len = 0;
static seq_t *R_seq = NULL;
static size_t R_seq_cap = 0;
static bool *R_used = NULL;
static size_t R_used_cap = 0;

static void R_ensure_used(size_t min_index)
{
    if (min_index <= R_used_cap)
        return;
    size_t new_cap = R_used_cap ? R_used_cap * 2 : 4;
    while (new_cap < min_index)
        new_cap <<= 1;
    bool *tmp = (bool*)realloc(R_used, (new_cap + 1) * sizeof(bool));
    for (size_t i = R_used_cap + 1; i <= new_cap; ++i)
        tmp[i] = false;
    R_used = tmp;
    R_used_cap = new_cap;
}

static seq_t A121216_a(idx_t n)
{
    if (n == 0)
        return 0;
    if (R_seq_len == 0) {
        R_seq_cap = 4;
        R_seq = mem_new(seq_t, R_seq_cap);
        R_seq[0] = (seq_t)1;
        R_seq[1] = (seq_t)2;
        R_seq_len = 2;
        R_used_cap = 4;
        R_used = (bool*)calloc(R_used_cap + 1, sizeof(bool));
        R_used[1] = true;
        R_used[2] = true;
    }
    while (R_seq_len < (size_t)n) {
        seq_t prevprev = R_seq[R_seq_len - 2];
        size_t cand = 1;
        seq_t val;
        for (;;) {
            R_ensure_used(cand);
            if (!R_used[cand] && gcd((seq_t)cand, prevprev) == (seq_t)1) {
                val = (seq_t)cand;
                break;
            }
            ++cand;
        }
        if (R_seq_len == R_seq_cap) {
            R_seq_cap *= 2;
            R_seq = mem_resize(seq_t, R_seq, R_seq_cap);
        }
        R_seq[R_seq_len] = val;
        ++R_seq_len;
        R_used[cand] = true;
    }
    return R_seq[(size_t)(n - 1)];
}

static idx_t *rec_idx = NULL;
static size_t rec_cnt = 0;
static size_t rec_cap = 0;
static idx_t scan_pos = 0;
static seq_t scan_max = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (rec_cnt < (size_t)n) {
        ++scan_pos;
        seq_t v = A121216_a(scan_pos);
        if (v > scan_max) {
            scan_max = v;
            if (rec_cnt == rec_cap) {
                rec_cap = rec_cap ? rec_cap * 2 : 64;
                rec_idx = (idx_t*)realloc(rec_idx, rec_cap * sizeof(idx_t));
            }
            rec_idx[rec_cnt++] = scan_pos;
        }
    }
    return (seq_t)rec_idx[(size_t)(n - 1)];
}
