/*
 * A355773  Numbers all of whose divisors are members of A333369.
 * https://oeis.org/A355773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th odd number m such that m and every divisor of m have
 *            each odd digit an odd number of times and each even digit an
 *            even number of times (A333369); computed by search
 * Range:     u8 n <= 38, u16 n <= 619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9986 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A355773_simber(seq_t x)
{
    unsigned p = 0, q = 0;
    if (x == 0) return false;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        x /= 10;
        p ^= 1u << d;
        q |= 1u << d;
    }
    return p == (q & 0x2AAu);
}

static bool A355773_ok(seq_t m)
{
    if (!A355773_simber(m)) return false;
    for (seq_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            if (!A355773_simber(d)) return false;
            if (!A355773_simber(m / d)) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t next = 1;

    if (n == 0) return 0;
    if (!cache) {
        cap = 64;
        cache = mem_new(seq_t, cap);
    }
    while (len < (size_t)n) {
        if (A355773_ok(next)) {
            if (len >= cap) {
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = next;
        }
        next += 2;
    }
    return cache[(size_t)n - 1];
}
