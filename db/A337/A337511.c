/*
 * A337511  A333663(n) is the a(n)-th term in A333663 with its leading digit.
 * https://oeis.org/A337511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   search: generate A333663 (a(1)=1, then the smallest unused
 *            square whose leading digit is the last nonzero digit of the
 *            previous term); a(n) = number of terms A333663(1..n) with the
 *            same leading digit as A333663(n)
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9882 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint32_t *A337511_res = NULL;
static size_t A337511_size = 0, A337511_cap = 0;
static seq_t A337511_prev;
static seq_t A337511_nextm[10];
static uint32_t A337511_cnt[10];
static bool A337511_init = false;

static int A337511_lead(seq_t v)
{
    while (v >= (seq_t)10) v /= (seq_t)10;
    return (int)(v % (seq_t)10);
}

static int A337511_lastnz(seq_t v)
{
    while (v % (seq_t)10 == 0) v /= (seq_t)10;
    return (int)(v % (seq_t)10);
}

static void A337511_push(seq_t val)
{
    int d = A337511_lead(val);
    if (A337511_size >= A337511_cap) {
        size_t nc = A337511_cap ? A337511_cap * 2 : 1024;
        A337511_res = (uint32_t *)realloc(A337511_res, nc * sizeof(uint32_t));
        A337511_cap = nc;
    }
    A337511_cnt[d]++;
    A337511_res[A337511_size++] = A337511_cnt[d];
    A337511_prev = val;
}

seq_t a(idx_t n)
{
    if (!A337511_init) {
        for (int d = 0; d < 10; ++d) { A337511_nextm[d] = (seq_t)1; A337511_cnt[d] = 0; }
        A337511_push((seq_t)1);
        /* square 1 = 1^2 (leading digit 1) is now used */
        A337511_nextm[1] = (seq_t)2;
        A337511_init = true;
    }
    while (A337511_size < (size_t)n) {
        int target = A337511_lastnz(A337511_prev);
        seq_t m = A337511_nextm[target];
        seq_t sq;
        for (;;) {
            sq = m * m;
            if (A337511_lead(sq) == target) break;
            ++m;
        }
        A337511_nextm[target] = m + 1;
        A337511_push(sq);
    }
    return (seq_t)A337511_res[(size_t)n - 1];
}
