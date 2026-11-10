/*
 * A375769  The indices of the terms of A375768 in the Fibonacci sequence.
 * https://oeis.org/A375769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A130233(A375768(n)); scan integers x = 1, 2, 3, ..., keep
 *            those whose maximal prime exponent e is a Fibonacci number (e =
 *            0 for x = 1), and output the index k of e (e = 1 gives 2, e = 0
 *            gives 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9734 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *c375769_cache = NULL;
static size_t c375769_size = 0;
static size_t c375769_cap = 0;
static uint64_t c375769_next = 1;

static int c375769_maxexp(uint64_t x)
{
    int best = 0;
    uint64_t p;
    for (p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            int c = 0;
            while (x % p == 0) { x /= p; ++c; }
            if (c > best) best = c;
        }
    }
    if (x > 1 && best < 1) best = 1;
    return best;
}

/* returns Fibonacci index (A130233 convention, 1 -> 2) or -1 if e not Fibonacci */
static int c375769_fibidx(int e)
{
    if (e == 0) return 0;
    if (e == 1) return 2;
    int a = 1, b = 2, k = 3;
    while (b < e) {
        int c = a + b;
        a = b;
        b = c;
        ++k;
    }
    return (b == e) ? k : -1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t need = (size_t)n;
    if (c375769_cap < need) {
        size_t nc = c375769_cap ? c375769_cap : 64;
        while (nc < need) nc *= 2;
        seq_t *p = mem_resize(seq_t, c375769_cache, nc);
        if (!p) abort();
        c375769_cache = p;
        c375769_cap = nc;
    }
    while (c375769_size < need) {
        int k = c375769_fibidx(c375769_maxexp(c375769_next));
        if (k >= 0) {
            c375769_cache[c375769_size++] = (seq_t)k;
        }
        ++c375769_next;
    }
    return c375769_cache[need - 1];
}
