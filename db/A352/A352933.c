/*
 * A352933  First differences of A121216.
 * https://oeis.org/A352933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A121216(n+1) - A121216(n); A121216 is built by greedy
 *            search: b(1)=1, b(2)=2, b(n)=smallest unused positive integer
 *            coprime to b(n-2)
 * Range:     i8 n <= 450, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9926 (2026-11-10)
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

static size_t A121216_seq_len = 0;
static seq_t *A121216_seq = NULL;
static size_t A121216_seq_cap = 0;

static bool *A121216_used = NULL;
static size_t A121216_used_cap = 0;

static void A121216_ensure_used_capacity(idx_t min_index)
{
    if ((size_t)min_index <= A121216_used_cap)
        return;

    size_t new_cap = A121216_used_cap ? A121216_used_cap * 2 : 4;
    while (new_cap < (size_t)min_index)
        new_cap <<= 1;

    bool *tmp = (bool*)realloc(A121216_used, (new_cap + 1) * sizeof(bool));
    for (size_t i = A121216_used_cap + 1; i <= new_cap; ++i)
        tmp[i] = false;
    A121216_used = tmp;
    A121216_used_cap = new_cap;
}

static seq_t A121216_a(idx_t n)
{
    if (n == 0)
        return 0;

    if (A121216_seq_len == 0) {
        A121216_seq_cap = 4;
        A121216_seq = mem_new(seq_t, A121216_seq_cap);
        A121216_seq[0] = (seq_t)1;
        A121216_seq[1] = (seq_t)2;
        A121216_seq_len = 2;

        A121216_used_cap = 4;
        A121216_used = (bool*)calloc(A121216_used_cap + 1, sizeof(bool));
        A121216_used[1] = true;
        A121216_used[2] = true;
    }

    while (A121216_seq_len < (size_t)n) {
        seq_t prevprev = A121216_seq[A121216_seq_len - 2];
        idx_t cand = 1;
        seq_t val;

        for (;;) {
            A121216_ensure_used_capacity(cand);
            if (!A121216_used[(size_t)cand] && gcd((seq_t)cand, prevprev) == (seq_t)1) {
                val = (seq_t)cand;
                break;
            }
            ++cand;
        }

        if (A121216_seq_len == A121216_seq_cap) {
            A121216_seq_cap *= 2;
            A121216_seq = mem_resize(seq_t, A121216_seq, A121216_seq_cap);
        }
        A121216_seq[A121216_seq_len] = val;
        ++A121216_seq_len;

        A121216_used[(size_t)cand] = true;
    }

    return A121216_seq[(size_t)(n - 1)];
}

seq_t a(idx_t n)
{
    seq_t hi = A121216_a(n + 1);
    seq_t lo = A121216_a(n);
    return hi - lo;
}
