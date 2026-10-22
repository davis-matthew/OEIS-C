/*
 * A397817  Lexicographically earliest increasing sequence of nonnegative
 *          integers such that no two distinct terms differ by a perfect cube.
 * https://oeis.org/A397817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest increasing sequence of nonnegative
 *            integers such that no two distinct terms differ by a perfect
 *            cube. The recursive/greedy construction is: start with a(0)=0.
 *            For n>0 choose the smallest integer m > a(n-1) such that for all
 *            i<n, m - a(i) is not a perfect cube
 * Range:     u8 n <= 89, u16 n <= 8653, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for memoization */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint8_t *used = NULL;
static size_t used_cap = 0;

/* grow an array of seq_t values */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* grow an array of used flags */
static void ensure_used_cap(idx_t needed)
{
    if (needed < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 8;
    while (new_cap <= needed)
        new_cap <<= 1;
    used = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    /* mark newly allocated part as unused (0) */
    for (size_t i = used_cap; i < new_cap; ++i)
        used[i] = 0;
    used_cap = new_cap;
}

/* generate next term of the sequence */
static void generate_next(void)
{
    seq_t last = seq[seq_len - 1];
    idx_t cand_idx = (idx_t)last + 1;
    seq_t cand = (seq_t)cand_idx;
    while (true) {
        /* ensure used table covers cand */
        ensure_used_cap(cand_idx);
        bool forbidden = false;
        /* check all perfect cubes <= cand */
        for (idx_t k = 1;; ++k) {
            idx_t cube;
            /* compute k^3 with overflow guard, but k is small (< 1e4) */
            cube = k * k * k;
            if (cube > cand_idx)
                break;
            if (used[cand_idx - cube]) {
                forbidden = true;
                break;
            }
        }
        if (!forbidden) {
            /* accept cand */
            ensure_seq_cap(seq_len + 1);
            seq[seq_len] = cand;
            seq_len++;
            used[cand_idx] = 1;
            break;
        }
        /* try next candidate */
        cand_idx += 1;
        cand = (seq_t)cand_idx;
    }
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (seq_len == 0) {
        /* initial term a(0)=0 */
        ensure_seq_cap(1);
        seq[0] = (seq_t)0;
        seq_len = 1;
        ensure_used_cap(0);
        used[0] = 1;
    }
    while (seq_len <= n)
        generate_next();
    return seq[n];
}
