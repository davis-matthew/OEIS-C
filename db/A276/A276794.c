/*
 * A276794  Indicator function for A003145.
 * https://oeis.org/A276794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n+1) = t(n)*(2 - t(n)), where t(n) = A080843(n) is
 *            the tribonacci word 0,1,0,2,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8921 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A276794_lens = NULL;
static size_t A276794_cnt = 0, A276794_cap = 0;

static void A276794_ensure(uint64_t target)
{
    while (A276794_cnt == 0 || A276794_lens[A276794_cnt - 1] <= target) {
        if (A276794_cnt == A276794_cap) {
            size_t nc = A276794_cap ? A276794_cap << 1 : 8;
            uint64_t *tmp = (uint64_t *)realloc(A276794_lens, nc * sizeof(uint64_t));
            if (!tmp) exit(1);
            A276794_lens = tmp;
            A276794_cap = nc;
        }
        if (A276794_cnt == 0) A276794_lens[A276794_cnt++] = 1;
        else if (A276794_cnt == 1) A276794_lens[A276794_cnt++] = 2;
        else if (A276794_cnt == 2) A276794_lens[A276794_cnt++] = 4;
        else {
            A276794_lens[A276794_cnt] = A276794_lens[A276794_cnt - 1]
                                      + A276794_lens[A276794_cnt - 2]
                                      + A276794_lens[A276794_cnt - 3];
            A276794_cnt++;
        }
    }
}

static int A276794_t(uint64_t pos)
{
    A276794_ensure(pos);
    size_t k = A276794_cnt - 1;
    while (k > 2) {
        uint64_t l1 = A276794_lens[k - 1];
        uint64_t l2 = A276794_lens[k - 2];
        if (pos < l1) {
            k -= 1;
        } else if (pos < l1 + l2) {
            pos -= l1;
            k -= 2;
        } else {
            pos -= l1 + l2;
            k -= 3;
        }
    }
    if (k == 0) return 0;
    if (k == 1) return pos == 0 ? 0 : 1;
    if (pos == 0) return 0;
    if (pos == 1) return 1;
    if (pos == 2) return 0;
    return 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    int t = A276794_t((uint64_t)(n - 1));
    return (seq_t)(t * (2 - t));
}
