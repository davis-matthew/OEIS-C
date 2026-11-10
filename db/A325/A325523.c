/*
 * A325523  Difference sequence of A325430.
 * https://oeis.org/A325523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325430(n+1) - A325430(n); A325430 = numbers marked by
 *            sieve: for i=1,2,3,... if i is unmarked and i>1, mark 3i and
 *            floor(3i/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9726 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A325523_seq = NULL;
static idx_t A325523_len = 0;
static idx_t A325523_cap = 0;

static uint8_t *A325523_marked = NULL;
static idx_t A325523_mcap = 0;

static idx_t A325523_next = 1;

static void A325523_ensure_marked(idx_t idx)
{
    if (idx < A325523_mcap) return;
    idx_t new_cap = A325523_mcap ? A325523_mcap << 1 : 128;
    while (new_cap <= idx) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(A325523_marked, (size_t)new_cap);
    if (!tmp) exit(1);
    memset(tmp + A325523_mcap, 0, (size_t)(new_cap - A325523_mcap));
    A325523_marked = tmp;
    A325523_mcap = new_cap;
}

static void A325523_ensure_seq(void)
{
    if (A325523_len < A325523_cap) return;
    idx_t new_cap = A325523_cap ? A325523_cap << 1 : 128;
    seq_t *tmp = mem_resize(seq_t, A325523_seq, new_cap);
    if (!tmp) exit(1);
    A325523_seq = tmp;
    A325523_cap = new_cap;
}

static void A325523_build(idx_t count)
{
    while (A325523_len < count) {
        A325523_ensure_marked(A325523_next);
        if (A325523_marked[A325523_next]) {
            A325523_ensure_seq();
            A325523_seq[A325523_len++] = (seq_t)A325523_next;
        } else if (A325523_next > 1) {
            idx_t i1 = A325523_next * 3;
            idx_t i2 = (A325523_next * 3) / 2;
            A325523_ensure_marked(i1);
            A325523_marked[i1] = 1;
            A325523_ensure_marked(i2);
            A325523_marked[i2] = 1;
        }
        ++A325523_next;
    }
}

seq_t a(idx_t n)
{
    A325523_build(n + 1);
    return A325523_seq[n] - A325523_seq[n - 1];
}
