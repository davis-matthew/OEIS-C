/*
 * A375767  The indices of the terms of A375766 in the Fibonacci sequence.
 * https://oeis.org/A375767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A130233(A375766(n)); search: enumerate integers whose
 *            prime exponents are all Fibonacci numbers, take the max exponent
 *            m, return the index k of m in the Fibonacci numbers (0->0, 1->2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9654 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A375767_isfib(uint64_t e, unsigned *idx)
{
    uint64_t p = 0, c = 1;
    unsigned i = 1;
    if (e == 0) { *idx = 0; return true; }
    while (c < e) {
        uint64_t nx = p + c;
        p = c; c = nx; i++;
    }
    if (c == e) {
        *idx = (e == 1) ? 2 : i;
        return true;
    }
    return false;
}

static bool A375767_good(uint64_t x, unsigned *res)
{
    uint64_t t = x;
    uint64_t maxE = 0;
    unsigned id;
    uint64_t p;
    for (p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; e++; }
            if (!A375767_isfib(e, &id)) return false;
            if (e > maxE) maxE = e;
        }
    }
    if (t > 1 && maxE < 1) maxE = 1;
    A375767_isfib(maxE, &id);
    *res = id;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0, count = 0;
    static uint64_t next = 1;
    if (n == 0) return 0;
    while (count < (size_t)n) {
        unsigned r;
        if (A375767_good(next, &r)) {
            if (count == cap) {
                size_t nc = cap ? cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, nc);
                if (!tmp) return 0;
                cache = tmp;
                cap = nc;
            }
            cache[count++] = (seq_t)r;
        }
        next++;
    }
    return cache[(size_t)(n - 1)];
}
