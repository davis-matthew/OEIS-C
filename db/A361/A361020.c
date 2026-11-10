/*
 * A361020  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A343029(i) = A343029(j) and A343030(i) = A343030(j) for all i,
 *          j >= 0.
 * https://oeis.org/A361020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted growth transform of the pairs [A343029(n),
 *            A343030(n)]: a(n) = 1 + number of distinct pairs first seen
 *            before the pair at n, scanning from n = 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9702 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void A361020_pair(idx_t n, unsigned *e, unsigned *o)
{
    unsigned zeros = 0, ev = 0, od = 0;
    while (n) {
        if (n & 1ULL) {
            if (zeros & 1u) od++; else ev++;
        } else {
            zeros++;
        }
        n >>= 1;
    }
    *e = ev;
    *o = od;
}

static uint32_t *A361020_vals = 0;
static idx_t A361020_cnt = 0;
static idx_t A361020_cap = 0;
static unsigned A361020_pe[8192], A361020_po[8192];
static unsigned A361020_np = 0;

seq_t a(idx_t n)
{
    if (n >= A361020_cap) {
        idx_t nc = A361020_cap ? A361020_cap : 1024;
        while (nc <= n) nc *= 2;
        A361020_vals = (uint32_t *)realloc(A361020_vals, (size_t)nc * sizeof(uint32_t));
        A361020_cap = nc;
    }
    while (A361020_cnt <= n) {
        unsigned e, o, k;
        A361020_pair(A361020_cnt, &e, &o);
        for (k = 0; k < A361020_np; k++)
            if (A361020_pe[k] == e && A361020_po[k] == o) break;
        if (k == A361020_np) {
            A361020_pe[k] = e;
            A361020_po[k] = o;
            A361020_np++;
        }
        A361020_vals[A361020_cnt] = k + 1;
        A361020_cnt++;
    }
    return (seq_t)A361020_vals[n];
}
