/*
 * A277711  a(n) = position of the first occurrence of n in A264977.
 * https://oeis.org/A277711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of first occurrence of n in A264977, where
 *            A264977(0)=0, A264977(1)=1, A264977(2k)=2*A264977(k),
 *            A264977(2k+1)=A264977(k) XOR A264977(k+1); computed by searching
 *            a growing table
 * Range:     u8 n <= 33, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: standard, pass, 95 terms through n = 911 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A277711_tab = 0;
static size_t A277711_size = 0;

static void A277711_build(size_t sz)
{
    seq_t *t = mem_new(seq_t, sz);
    size_t i;
    t[0] = 0;
    if (sz > 1) t[1] = 1;
    for (i = 2; i < sz; i++) {
        if ((i & 1) == 0) t[i] = 2 * t[i >> 1];
        else t[i] = t[i >> 1] ^ t[(i >> 1) + 1];
    }
    if (A277711_tab) mem_free(A277711_tab);
    A277711_tab = t;
    A277711_size = sz;
}

seq_t a(idx_t n)
{
    const size_t CAP = (size_t)1 << 24;
    size_t start = 0, i;
    seq_t target = (seq_t)n;
    if (A277711_size == 0) A277711_build(1024);
    for (;;) {
        for (i = start; i < A277711_size; i++) {
            if (A277711_tab[i] == target) return (seq_t)i;
        }
        if (A277711_size >= CAP) return 0;
        start = A277711_size;
        A277711_build(A277711_size * 2);
    }
}
