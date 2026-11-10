/*
 * A298297  Numbers k such that b(k+1) = b(k) + 1, where b = A298296.
 * https://oeis.org/A298297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with b(k+1) = b(k)+1 where b = A298296
 *            (1-indexed: b(1)=3); b built by complementary sieve: b(n) =
 *            least unused integer, a(0)=1, a(1)=2, a(n)=b(n)+2*b(n-1),
 *            b(0..2)=3,4,5
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint8_t *A298297_used = NULL;
static size_t A298297_ub = 0;
static seq_t *A298297_b = NULL;
static size_t A298297_cap = 0;
static idx_t A298297_max = 0;
static uint64_t A298297_next = 6;
static int A298297_init = 0;

static void A298297_mark(uint64_t v)
{
    size_t need = (size_t)(v >> 3) + 1;
    if (need > A298297_ub) {
        size_t nb = A298297_ub > 0 ? A298297_ub : 16;
        while (nb < need) nb <<= 1;
        uint8_t *t = (uint8_t *)realloc(A298297_used, nb);
        if (!t) abort();
        for (size_t i = A298297_ub; i < nb; ++i) t[i] = 0;
        A298297_used = t;
        A298297_ub = nb;
    }
    A298297_used[v >> 3] |= (uint8_t)(1U << (v & 7));
}

static int A298297_isused(uint64_t v)
{
    if ((size_t)(v >> 3) >= A298297_ub) return 0;
    return (A298297_used[v >> 3] >> (v & 7)) & 1U;
}

static void A298297_gen(idx_t m)
{
    if (!A298297_init) {
        A298297_cap = 16;
        A298297_b = mem_new(seq_t, A298297_cap);
        A298297_mark(1); A298297_mark(2);
        A298297_b[0] = 3; A298297_b[1] = 4; A298297_b[2] = 5;
        A298297_mark(3); A298297_mark(4); A298297_mark(5);
        A298297_mark(13);
        A298297_max = 2;
        A298297_next = 6;
        A298297_init = 1;
    }
    while (A298297_max < m) {
        idx_t i = A298297_max + 1;
        if ((size_t)i >= A298297_cap) {
            size_t nc = A298297_cap * 2;
            seq_t *t = mem_resize(seq_t, A298297_b, nc);
            if (!t) abort();
            A298297_b = t;
            A298297_cap = nc;
        }
        while (A298297_isused(A298297_next)) ++A298297_next;
        seq_t bv = (seq_t)A298297_next;
        A298297_b[i] = bv;
        A298297_mark(A298297_next);
        ++A298297_next;
        seq_t av = bv + (seq_t)2 * A298297_b[i - 1];
        A298297_mark((uint64_t)av);
        A298297_max = i;
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        A298297_gen(k);
        if (A298297_b[k] - A298297_b[k - 1] == (seq_t)1)
            ++count;
    }
    return (seq_t)k;
}
