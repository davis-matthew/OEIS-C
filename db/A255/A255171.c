/*
 * A255171  First differences of A072473.
 * https://oeis.org/A255171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A072473(n+1) - A072473(n), where A072473(n) = prime(2n) -
 *            prime(n); computed by incremental prime generation
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 966 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A255171_pl = NULL;
static size_t A255171_cap = 0;
static size_t A255171_cnt = 0;

static void A255171_ensure(size_t needed)
{
    if (A255171_cnt >= needed)
        return;
    if (A255171_cap < needed) {
        size_t nc = A255171_cap ? A255171_cap * 2 : 16;
        if (nc < needed)
            nc = needed;
        void *t = realloc(A255171_pl, nc * sizeof(uint64_t));
        if (!t)
            abort();
        A255171_pl = (uint64_t *)t;
        A255171_cap = nc;
    }
    uint64_t last = (A255171_cnt == 0) ? 1 : A255171_pl[A255171_cnt - 1];
    while (A255171_cnt < needed) {
        ++last;
        int ip = 1;
        for (size_t i = 0; i < A255171_cnt; ++i) {
            uint64_t p = A255171_pl[i];
            if (p * p > last)
                break;
            if (last % p == 0) {
                ip = 0;
                break;
            }
        }
        if (ip)
            A255171_pl[A255171_cnt++] = last;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    A255171_ensure((size_t)(2 * n + 2));
    seq_t pn   = (seq_t)A255171_pl[n - 1];
    seq_t pn1  = (seq_t)A255171_pl[n];
    seq_t p2n  = (seq_t)A255171_pl[2 * n - 1];
    seq_t p2n2 = (seq_t)A255171_pl[2 * n + 1];
    return (p2n2 - pn1) - (p2n - pn);
}
