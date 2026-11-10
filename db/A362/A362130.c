/*
 * A362130  a(n) = A000005(A360179(n)) mod 2.
 * https://oeis.org/A362130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A360179(n)) mod 2, i.e. 1 iff A360179(n) is a
 *            perfect square; A360179 is computed by direct simulation of its
 *            recurrence
 * Range:     u8 n <= 5512, u16 n <= 40000, u32 n <= 40000, u64 n <= 40000,
 *            u128 n <= 40000, big any n
 * Validated: standard, pass, 136 terms through n = 9861 (2026-11-10)
 * Helpers:   is_square, mem, num_divisors_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/num_divisors_seq.h"
#include "../utils/is_square.h"

static seq_t *A360179_seq = 0;
static bool *A360179_starred = 0;
static idx_t A360179_cap = 0;
static idx_t A360179_len = 0;

static seq_t A360179_a(idx_t n)
{
    if (n > A360179_cap) {
        idx_t nc = n * 2;
        if (nc < 1024) nc = 1024;
        if (A360179_seq) mem_free(A360179_seq);
        if (A360179_starred) mem_free(A360179_starred);
        A360179_seq = mem_new(seq_t, nc + 2);
        A360179_starred = mem_new(bool, nc + 2);
        A360179_cap = nc;
        A360179_len = 0;
    }
    if (A360179_len == 0) {
        A360179_seq[1] = (seq_t)1;
        A360179_starred[1] = false;
        A360179_len = 1;
    }
    for (idx_t i = A360179_len + 1; i <= n; ++i) {
        seq_t prev = A360179_seq[i - 1];
        bool novel = true;
        for (idx_t j = 1; j + 1 < i; ++j) {
            if (A360179_seq[j] == prev) { novel = false; break; }
        }
        if (novel) {
            A360179_seq[i] = num_divisors_seq(prev);
        } else {
            bool found = false;
            idx_t bi = 0;
            for (idx_t j = 1; j + 1 < i; ++j) {
                if (!A360179_starred[j]) {
                    if (!found || A360179_seq[j] < A360179_seq[bi]) {
                        bi = j;
                        found = true;
                    }
                }
            }
            A360179_starred[bi] = true;
            A360179_seq[i] = prev + num_divisors_seq(A360179_seq[bi]);
        }
        A360179_starred[i] = false;
    }
    if (n > A360179_len) A360179_len = n;
    return A360179_seq[n];
}

seq_t a(idx_t n)
{
    seq_t v = A360179_a(n);
    return is_square(v) ? (seq_t)1 : (seq_t)0;
}
