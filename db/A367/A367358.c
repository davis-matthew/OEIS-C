/*
 * A367358  Indices k at which either the leading digit or the length of
 *          A121805(k) changes.
 * https://oeis.org/A367358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   search -- run the A121805 recurrence a(1)=1, a(k) = a(k-1) +
 *            10*(last digit of a(k-1)) + y, where y is the smallest digit
 *            equal to the leading digit of a(k), until no such y exists. a(1)
 *            = 1, and the later terms are the indices k where the leading
 *            digit or the length of A121805(k) differs from A1...
 * Range:     u8 n <= 23, u16 n <= 43, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A367358_list = NULL;
static size_t A367358_cnt = 0;
static uint64_t A367358_lastidx = 0;
static bool A367358_done = false;

static void A367358_info(uint64_t x, int *lead, int *len)
{
    int l = 1;
    while (x >= 10) { x /= 10; l++; }
    *lead = (int)x;
    *len = l;
}

static void A367358_push(uint64_t v, size_t *cap)
{
    if (A367358_cnt == *cap) {
        size_t nc = (*cap == 0) ? 64 : (*cap * 2);
        uint64_t *p = (uint64_t *)realloc(A367358_list, nc * sizeof(uint64_t));
        if (!p) return;
        A367358_list = p;
        *cap = nc;
    }
    A367358_list[A367358_cnt++] = v;
}

static void A367358_build(void)
{
    size_t cap = 0;
    uint64_t prev = 1;
    uint64_t k = 1;
    int plead, plen;
    A367358_info(prev, &plead, &plen);
    A367358_push(1, &cap);              /* k = 1 is always a term */
    for (;;) {
        uint64_t base = prev + (prev % 10) * 10;
        uint64_t val = 0;
        for (int d = 1; d <= 9; ++d) {
            uint64_t cand = base + (uint64_t)d;
            int l, ln;
            A367358_info(cand, &l, &ln);
            if (l == d) { val = cand; break; }
        }
        if (val == 0) break;            /* A121805 terminates */
        ++k;
        int lead, len;
        A367358_info(val, &lead, &len);
        if (lead != plead || len != plen)
            A367358_push(k, &cap);
        prev = val;
        plead = lead;
        plen = len;
    }
    A367358_lastidx = k;
    A367358_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!A367358_done) A367358_build();
    if (n <= (idx_t)A367358_cnt) return (seq_t)A367358_list[n - 1];
    if (n == (idx_t)A367358_cnt + 1 &&
        A367358_list[A367358_cnt - 1] != A367358_lastidx)
        return (seq_t)A367358_lastidx;  /* final index of the finite A121805 */
    return (seq_t)0;
}
