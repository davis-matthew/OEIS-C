/*
 * A391579  Fixed points of A391573.
 * https://oeis.org/A391579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m such that A391573(m) = m, where A391573(m) = (m
 *            carryless* k)/k for the smallest odd k>1 with k | (m carryless*
 *            k); found by a incremental search with a cache
 * Range:     u8 n <= 31, u16 n <= 579, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 8534 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static idx_t *A391579_fp = 0;
static size_t A391579_cnt = 0, A391579_cap = 0;
static idx_t A391579_next = 1;

static bool A391579_is_fixed(idx_t m)
{
    seq_t mv = (seq_t)m;
    for (idx_t k = 3; ; k += 2) {
        seq_t prod = (seq_t)0;
        idx_t kk = k;
        unsigned shift = 0;
        while (kk != 0) {
            if (kk & 1ULL)
                prod ^= (mv << shift);
            kk >>= 1;
            shift++;
        }
        if ((prod % (seq_t)k) == 0)
            return prod == mv * (seq_t)k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while (A391579_cnt < (size_t)n) {
        idx_t m = A391579_next++;
        if (A391579_is_fixed(m)) {
            if (A391579_cnt == A391579_cap) {
                size_t nc = A391579_cap ? A391579_cap * 2 : 1024;
                A391579_fp = (idx_t *)realloc(A391579_fp, nc * sizeof(idx_t));
                A391579_cap = nc;
            }
            A391579_fp[A391579_cnt++] = m;
        }
    }
    return (seq_t)A391579_fp[n - 1];
}
