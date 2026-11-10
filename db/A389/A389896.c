/*
 * A389896  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A007814(i) = A007814(j), A343029(i) = A343029(j) and A343030(i)
 *          = A343030(j) for all i, j >= 1.
 * https://oeis.org/A389896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = restricted growth transform of the triple [A007814(n),
 *            A343029(n), A343030(n)]: scan n=1,2,..., giving each new triple
 *            the next unused label 1,2,3,
 * Range:     u8 n <= 3807, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 9717 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A389896_res = 0;
static size_t A389896_cnt = 0, A389896_cap = 0;
static uint32_t A389896_ids[64 * 65 * 65];
static uint32_t A389896_next = 1;

static uint32_t A389896_key(uint64_t n)
{
    uint32_t v = 0, c0 = 0, c1 = 0, zeros = 0;
    uint64_t t = n;
    while (t) {
        if (t & 1ULL) {
            if (zeros & 1) c1++; else c0++;
        } else {
            zeros++;
            if (c0 == 0 && c1 == 0) v++;
        }
        t >>= 1;
    }
    return (v * 65u + c0) * 65u + c1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A389896_cnt < (size_t)n) {
        if (A389896_cnt == A389896_cap) {
            size_t nc = A389896_cap ? A389896_cap * 2 : 1024;
            A389896_res = (uint32_t *)realloc(A389896_res, nc * sizeof(uint32_t));
            A389896_cap = nc;
        }
        uint64_t m = (uint64_t)A389896_cnt + 1;
        uint32_t k = A389896_key(m);
        if (A389896_ids[k] == 0) A389896_ids[k] = A389896_next++;
        A389896_res[A389896_cnt++] = A389896_ids[k];
    }
    return (seq_t)A389896_res[n - 1];
}
