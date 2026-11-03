/*
 * A257020  Numbers whose quarter-square representation consists of three
 *          terms.
 * https://oeis.org/A257020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 964 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: for each integer m, repeatedly subtract the largest
 * quarter square ≤ the remainder (the greedy algorithm).
 * If the subtraction terminates with exactly three distinct terms,
 * m is stored.  The function returns the nth such integer (1‑based).
 * This reproduces OEIS A257020.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*  build a list of all quarter squares up to LIMIT (inclusive) */
static void
build_quarter_squares(seq_t **qs_out, size_t *cnt_out, seq_t LIMIT)
{
    size_t cap = 1024;
    seq_t *qs = mem_new(seq_t, cap);
    size_t cnt = 0;
    for (uint64_t n = 1; ; ++n) {
        seq_t s = (seq_t)n * (seq_t)n;
        if (s > LIMIT)
            break;
        if (cnt >= cap) {
            cap <<= 1;
            qs = mem_resize(seq_t, qs, cap);
        }
        qs[cnt++] = s;
        seq_t sn = s + (seq_t)n;
        if (sn <= LIMIT) {
            if (cnt >= cap) {
                cap <<= 1;
                qs = mem_resize(seq_t, qs, cap);
            }
            qs[cnt++] = sn;
        }
    }
    *qs_out = qs;
    *cnt_out = cnt;
}

/*  helper: binary search for last element <= x in a sorted array */
static size_t
index_of_max_leq(const seq_t *arr, size_t cnt, seq_t x)
{
    size_t lo = 0, hi = cnt - 1, best = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] <= x) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

/*  determine whether m has a greedy representation of exactly three terms   */
static bool
three_terms(const seq_t *qs, size_t qcnt, seq_t m)
{
    size_t terms = 0;
    seq_t rem = m;
    while (rem > 0 && terms < 4) {
        size_t idx = index_of_max_leq(qs, qcnt, rem);
        seq_t q = qs[idx];
        rem -= q;
        ++terms;
        if (rem == 0 && terms == 3)
            return true;
        if (rem == 0)
            return false;
    }
    return false;
}

/*  main sequence function                                                    */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *quarter_squares = NULL;
    static size_t quarter_cnt = 0;
    static seq_t *values = NULL;
    static size_t val_len = 0, val_cap = 0;
    static seq_t next_m = 1;          /* next integer to test */

    /* ensure quarter squares exist */
    if (!quarter_squares) {
        const seq_t LIMIT = (seq_t)100000000; /* 1e8 – ample for 10kth term */
        build_quarter_squares(&quarter_squares, &quarter_cnt, LIMIT);
    }

    /* produce values until we have nth */
    while (val_len < (size_t)n) {
        /* extend storage if necessary */
        if (val_len >= val_cap) {
            size_t new_cap = val_cap == 0 ? 128 : val_cap << 1;
            values = mem_resize(seq_t, values, new_cap);
            val_cap = new_cap;
        }
        if (three_terms(quarter_squares, quarter_cnt, next_m))
            values[val_len++] = next_m;
        ++next_m;
    }
    return values[n - 1];
}
