/*
 * A328049  First differences of A328047.
 * https://oeis.org/A328049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A328047(n+1) - A328047(n); A328047 is built by a greedy
 *            search (smallest unused integer with bigomega differing from the
 *            previous term)
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-08)
 * Helpers:   bigomega, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/mem.h"

static seq_t *A328047_seq = NULL;
static size_t A328047_len = 0;
static size_t A328047_cap = 0;

static uint8_t *A328047_used = NULL;
static size_t A328047_used_cap = 0;

static void A328047_ensure_seq_cap(size_t needed)
{
    if (needed <= A328047_cap) return;
    size_t new_cap = A328047_cap ? A328047_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = mem_resize(seq_t, A328047_seq, new_cap);
    A328047_seq = new_arr;
    A328047_cap = new_cap;
}

static void A328047_ensure_used_cap(size_t needed)
{
    if (needed < A328047_used_cap) return;
    size_t new_cap = A328047_used_cap ? A328047_used_cap * 2 : 128;
    while (new_cap <= needed) new_cap *= 2;
    uint8_t *new_arr = (uint8_t *)realloc(A328047_used, new_cap * sizeof(uint8_t));
    memset(new_arr + A328047_used_cap, 0, new_cap - A328047_used_cap);
    A328047_used = new_arr;
    A328047_used_cap = new_cap;
}

static void A328047_compute_next(void)
{
    if (A328047_len == 0) {
        A328047_ensure_seq_cap(1);
        A328047_seq[0] = (seq_t)1;
        A328047_ensure_used_cap(2);
        A328047_used[1] = 1;
        A328047_len = 1;
        return;
    }
    seq_t last_val = A328047_seq[A328047_len - 1];
    idx_t last_omega = (idx_t)bigomega((idx_t)last_val);
    idx_t candidate = 1;
    for (;;) {
        A328047_ensure_used_cap((size_t)candidate + 1);
        if (!A328047_used[(size_t)candidate]) {
            idx_t cand_omega = (idx_t)bigomega(candidate);
            if (cand_omega != last_omega) {
                A328047_ensure_seq_cap(A328047_len + 1);
                A328047_seq[A328047_len] = (seq_t)candidate;
                A328047_used[(size_t)candidate] = 1;
                A328047_len++;
                break;
            }
        }
        candidate++;
    }
}

static seq_t A328047_a(idx_t n)
{
    if (n == 0) return 0;
    while (A328047_len < (size_t)n) {
        A328047_compute_next();
    }
    return A328047_seq[n - 1];
}

seq_t a(idx_t n)
{
    seq_t hi = A328047_a(n + 1);
    seq_t lo = A328047_a(n);
    return hi - lo;
}
