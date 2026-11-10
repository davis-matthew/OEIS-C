/*
 * A347241  a(1) = 1, and for n > 1, a(n) is the largest prime factor
 *          (A006530) of all terms encountered when iterating the map x ->
 *          A000593(x), when starting from x = n, and including the n itself.
 *          If 1 is never reached when starting from n, then a(n) = -1.
 * https://oeis.org/A347241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(A006530(n), A347240(n)); computed by iterating x ->
 *            A000593(x) from n until 1 (or a cycle, giving -1), taking the
 *            largest prime factor over all terms
 * Range:     u8 n <= 227, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9372 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Given x >= 1, returns sum of odd divisors of x and sets *gpf to the greatest prime factor of x (1 if x == 1) */
static seq_t A347241_step(seq_t x, seq_t *gpf)
{
    seq_t g = 1;
    while ((x & 1) == 0) {
        x >>= 1;
        g = 2;
    }
    seq_t result = 1;
    for (seq_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) {
            seq_t sum = 1, power = 1;
            while (x % d == 0) {
                x /= d;
                power *= d;
                sum += power;
            }
            result *= sum;
            g = d;
        }
    }
    if (x > 1) {
        result *= (1 + x);
        g = x;
    }
    *gpf = g;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    const size_t CAP = 4096;
    seq_t *visited = mem_new(seq_t, CAP);
    size_t vlen = 0;
    seq_t cur = (seq_t)n;
    seq_t best = 1;
    seq_t ret;
    while (1) {
        if (cur == 1) { ret = best; break; }
        bool seen = false;
        for (size_t i = 0; i < vlen; ++i) {
            if (visited[i] == cur) { seen = true; break; }
        }
        if (seen || vlen >= CAP) { ret = (seq_t)(-1); break; }
        visited[vlen++] = cur;
        seq_t g;
        seq_t nxt = A347241_step(cur, &g);
        if (g > best) best = g;
        cur = nxt;
    }
    mem_free(visited);
    return ret;
}
